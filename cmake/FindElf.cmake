# Try to find libelf and headers. Define Elf_DIR if Elf is
# installed in a non-standard directory.
#
# This file sets the following variables:
#
# Elf_INCLUDE_DIR, where to find elf.h, etc.
# Elf_LIBRARIES, the libraries to link against
# Elf_FOUND, If false, do not try to use Elf.
#
# Also defined, but not for general use are:
# Elf_LIBRARY, the full path to the elf library.
# Elf_INCLUDE_PATH, for CMake backward compatibility

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_Elf QUIET libelf)
endif()

find_path(Elf_INCLUDE_DIR libelf.h
  HINTS
    ${PC_Elf_INCLUDEDIR}
    ${PC_Elf_INCLUDE_DIRS}
  PATHS
    /opt/homebrew/include
    /usr/local/include
    /usr/include
    ${Elf_DIR}/include
)

find_library(Elf_LIBRARY NAMES elf
  HINTS
    ${PC_Elf_LIBDIR}
    ${PC_Elf_LIBRARY_DIRS}
  PATHS
    /opt/homebrew/lib
    /usr/lib
    /usr/local/lib
    ${Elf_DIR}
  PATH_SUFFIXES lib lib64
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Elf DEFAULT_MSG
  Elf_LIBRARY Elf_INCLUDE_DIR)

if(Elf_FOUND AND NOT TARGET Elf::elf)
  add_library(Elf::elf UNKNOWN IMPORTED)
  
  set(Elf_INCLUDE_DIRS "${Elf_INCLUDE_DIR}")
  if(Elf_INCLUDE_DIR MATCHES "/libelf$")
    get_filename_component(Elf_PARENT_INCLUDE_DIR "${Elf_INCLUDE_DIR}" DIRECTORY)
    list(APPEND Elf_INCLUDE_DIRS "${Elf_PARENT_INCLUDE_DIR}")
  endif()

  set_target_properties(Elf::elf PROPERTIES
    IMPORTED_LOCATION "${Elf_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${Elf_INCLUDE_DIRS}"
  )
endif()

mark_as_advanced(
  Elf_INCLUDE_DIR
  Elf_LIBRARY)
