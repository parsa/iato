cmake_minimum_required(VERSION 3.5)
project(iato CXX)

enable_testing()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# --- Platform Detection & Global Definitions ---
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_definitions(OS_LINUX IATO _REENTRANT HAS_PTHREAD)
elseif(APPLE)
    add_compile_definitions(OS_DARWIN IATO _REENTRANT HAS_PTHREAD)
endif()

find_package(Threads REQUIRED)

# --- Compiler Flags ---
add_compile_options(-Wall -Wswitch -Woverloaded-virtual)

# Ensure -fPIC is enabled globally
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Define DEBUG macro for Debug builds
add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

find_package(Elf)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set(IATO_IA64_TST_SOURCE "${PROJECT_SOURCE_DIR}/misc/clt"
    CACHE STRING "Path to IA64 test binaries directory or tarball")
set(IATO_IA64_SPEC_SOURCE "${PROJECT_SOURCE_DIR}/misc/spec"
    CACHE STRING "Path to SPEC2000 binaries directory or tarball")
set(IATO_IA64_ASSET_DIR "${PROJECT_BINARY_DIR}/ia64_assets"
    CACHE PATH "Extraction directory for IA64 binary archives")

add_subdirectory(src/lib)
add_subdirectory(src/clt)

add_subdirectory(tst)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

file(MAKE_DIRECTORY "${IATO_IA64_ASSET_DIR}")

function(iato_resolve_ia64_source name source subdir out_dir)
  if(IS_DIRECTORY "${source}")
    file(REAL_PATH "${source}" resolved_path)
    set(${out_dir} "${resolved_path}" PARENT_SCOPE)
    return()
  endif()

  if(NOT EXISTS "${source}")
    message(FATAL_ERROR "IA64 ${name} source '${source}' does not exist")
  endif()

  file(REAL_PATH "${source}" archive_path)
  set(extract_dir "${IATO_IA64_ASSET_DIR}/${name}")
  set(marker "${extract_dir}/.${name}_extracted")
  if(NOT EXISTS "${marker}")
    file(MAKE_DIRECTORY "${extract_dir}")
    message(STATUS "Extracting ${archive_path} to ${extract_dir}")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E chdir "${extract_dir}" ${CMAKE_COMMAND} -E tar xzf "${archive_path}"
      RESULT_VARIABLE extract_rv
    )
    if(NOT extract_rv EQUAL 0)
      message(FATAL_ERROR "Failed to extract ${archive_path} (exit code ${extract_rv})")
    endif()
    file(WRITE "${marker}" "")
  endif()
  set(resolved_path "${extract_dir}/${subdir}")
  if(NOT IS_DIRECTORY "${resolved_path}")
    message(FATAL_ERROR "Expected directory '${resolved_path}' after extracting ${archive_path}")
  endif()
  file(REAL_PATH "${resolved_path}" final_path)
  set(${out_dir} "${final_path}" PARENT_SCOPE)
endfunction()

iato_resolve_ia64_source("tests" "${IATO_IA64_TST_SOURCE}" "clt" IATO_IA64_TST_DIR)
iato_resolve_ia64_source("spec" "${IATO_IA64_SPEC_SOURCE}" "spec" IATO_IA64_SPEC_DIR)

set(IATO_IA64_GENERATED_DIR "${PROJECT_BINARY_DIR}/ia64_generated")
file(MAKE_DIRECTORY "${IATO_IA64_GENERATED_DIR}")
set(IATO_IA64_MANIFEST "${IATO_IA64_GENERATED_DIR}/misc_ia64_binaries.json")
set(IATO_IA64_CMAKE_LIST "${IATO_IA64_GENERATED_DIR}/ia64_entries.cmake")
execute_process(
  COMMAND
    ${Python3_EXECUTABLE} "${PROJECT_SOURCE_DIR}/scripts/generate_ia64_manifest.py"
    --repo-root "${PROJECT_SOURCE_DIR}"
    --output "${IATO_IA64_MANIFEST}"
    --cmake-output "${IATO_IA64_CMAKE_LIST}"
    --root "clt=${IATO_IA64_TST_DIR}"
    --root "spec=${IATO_IA64_SPEC_DIR}"
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
  RESULT_VARIABLE IATO_MANIFEST_RESULT
)
if(NOT IATO_MANIFEST_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to generate IA64 manifest (exit code ${IATO_MANIFEST_RESULT})")
endif()
include("${IATO_IA64_CMAKE_LIST}")
if(NOT IATO_IA64_ENTRIES)
  message(FATAL_ERROR "No IA64 binaries found in provided sources.")
endif()

function(iato_slugify input out_var)
  string(REPLACE "/" "__" slug "${input}")
  string(REPLACE "." "_" slug "${slug}")
  string(REGEX REPLACE "[^A-Za-z0-9_]+" "_" slug "${slug}")
  set(${out_var} "${slug}" PARENT_SCOPE)
endfunction()

set(IATO_CLIENTS iaos iaka iaio iaoo iata)
set(IATO_CLIENT_iaos_BINARY "${PROJECT_BINARY_DIR}/bin/iaos")
set(IATO_CLIENT_iaos_FLAGS -e)
set(IATO_CLIENT_iaos_SPEC_FLAGS "")

set(IATO_CLIENT_iaka_BINARY "${PROJECT_BINARY_DIR}/bin/iaka")
set(IATO_CLIENT_iaka_FLAGS -s -m 10000)
set(IATO_CLIENT_iaka_SPEC_FLAGS -s -m 4000)

set(IATO_CLIENT_iaio_BINARY "${PROJECT_BINARY_DIR}/bin/iaio")
set(IATO_CLIENT_iaio_FLAGS -s -m:i 20000 -m:c 20000)
set(IATO_CLIENT_iaio_SPEC_FLAGS -s -m:i 6000 -m:c 6000)

set(IATO_CLIENT_iaoo_BINARY "${PROJECT_BINARY_DIR}/bin/iaoo")
set(IATO_CLIENT_iaoo_FLAGS -s -m:i 20000 -m:c 20000)
set(IATO_CLIENT_iaoo_SPEC_FLAGS -s -m:i 6000 -m:c 6000)

set(IATO_CLIENT_iata_BINARY "${PROJECT_BINARY_DIR}/bin/iata")
set(IATO_CLIENT_iata_FLAGS -s)
set(IATO_CLIENT_iata_SPEC_FLAGS "")

set(IATO_IA64_TRACE_ROOT "${PROJECT_BINARY_DIR}/traces/ia64")
file(MAKE_DIRECTORY "${IATO_IA64_TRACE_ROOT}/iaka")

function(iato_add_client_test client category abs_path rel_path slug label expected_pattern fixture_path)
  set(binary "${IATO_CLIENT_${client}_BINARY}")
  set(cmd "${binary}")
  set(base_flags "${IATO_CLIENT_${client}_FLAGS}")
  if(NOT "${base_flags}" STREQUAL "")
    list(APPEND cmd ${base_flags})
  endif()
  set(spec_flags "${IATO_CLIENT_${client}_SPEC_FLAGS}")
  if(category STREQUAL "spec" AND NOT "${spec_flags}" STREQUAL "")
    list(APPEND cmd ${spec_flags})
  endif()

  set(trace_path "${IATO_IA64_TRACE_ROOT}/iaka/${slug}.trc")
  if(client STREQUAL "iaka")
    list(APPEND cmd -t:f "${trace_path}")
  endif()

  if(NOT "${fixture_path}" STREQUAL "")
    if(client STREQUAL "iaio" OR client STREQUAL "iaoo" OR client STREQUAL "iaka")
      list(APPEND cmd -p:s "TEST-FIXTURE-FILE:string=${fixture_path}")
    endif()
  endif()

  if(client STREQUAL "iata")
    list(APPEND cmd "${trace_path}")
  else()
    list(APPEND cmd "${abs_path}")
  endif()

  if(NOT "${expected_pattern}" STREQUAL "")
    set(cmd
      ${Python3_EXECUTABLE} "${PROJECT_SOURCE_DIR}/scripts/expect_break.py"
      --match "${expected_pattern}" -- ${cmd})
  endif()

  set(formatted_name "${client}.ia64.${label}")
  add_test(NAME "${formatted_name}" COMMAND ${cmd})
  set_tests_properties("${formatted_name}"
    PROPERTIES WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
  if(client STREQUAL "iata")
    set(dep_test "iaka.ia64.${label}")
    set_tests_properties("${formatted_name}" PROPERTIES DEPENDS "${dep_test}")
  endif()
endfunction()

foreach(entry ${IATO_IA64_ENTRIES})
  string(REPLACE ":::" ";" entry_parts "${entry}")
  list(LENGTH entry_parts entry_len)
  if(NOT entry_len EQUAL 2)
    message(FATAL_ERROR "Malformed IA64 entry '${entry}'")
  endif()
  list(GET entry_parts 0 entry_category)
  list(GET entry_parts 1 entry_abs_path)
  if(entry_category STREQUAL "clt")
    file(RELATIVE_PATH entry_rel_path "${IATO_IA64_TST_DIR}" "${entry_abs_path}")
  else()
    # Track non-test categories (e.g., SPEC) but don't register CTest entries.
    continue()
  endif()

  iato_slugify("${entry_category}_${entry_rel_path}" entry_slug)

  set(entry_label_path "${entry_rel_path}")
  string(REPLACE "/" "." entry_label_path "${entry_label_path}")
  string(REPLACE " " "_" entry_label_path "${entry_label_path}")
  string(REPLACE ":" "_" entry_label_path "${entry_label_path}")
  string(REGEX REPLACE "\\.+"
    "." entry_label_path "${entry_label_path}")
  string(REGEX REPLACE "^\\.+"
    "" entry_label_path "${entry_label_path}")

  set(test_label_core "${entry_category}.${entry_label_path}")
  set(test_label "tst.${test_label_core}")
  set(entry_expected_pattern "")
  if(entry_rel_path STREQUAL "isa/F15" OR entry_rel_path STREQUAL "isa/I19")
    set(entry_expected_pattern "instruction break fault")
  endif()
  set(entry_fixture "")
  if(entry_rel_path STREQUAL "isa/M16")
    set(entry_fixture "${PROJECT_SOURCE_DIR}/tst/fixtures/clt/isa/M16.fixture")
  endif()
  foreach(client ${IATO_CLIENTS})
    iato_add_client_test("${client}" "${entry_category}" "${entry_abs_path}" "${entry_rel_path}" "${entry_slug}" "${test_label}" "${entry_expected_pattern}" "${entry_fixture}")
  endforeach()
endforeach()
