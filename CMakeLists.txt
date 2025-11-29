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

add_subdirectory(src/lib)
add_subdirectory(src/clt)

add_subdirectory(tst)
