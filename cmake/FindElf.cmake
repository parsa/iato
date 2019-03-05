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

find_path(Elf_INCLUDE_DIR libelf.h
  PATHS /usr/local/include
  /usr/include
  ${Elf_DIR}/include)

find_library(Elf_LIBRARY NAMES elf
  PATHS /usr
  /usr/local
  ${Elf_DIR}
  PATH_SUFFIXES lib lib64)

set(Elf_FOUND OFF)
if(Elf_INCLUDE_DIR)
  if(Elf_LIBRARY)
    set(Elf_LIBRARIES ${Elf_LIBRARY})
    set(Elf_FOUND ON)

  else()
    if(Elf_FIND_REQURIED)
      message(SEND_ERROR "Unable to find libelf.")
    endif()
  endif()
endif()

mark_as_advanced(
  Elf_INCLUDE_DIR
  Elf_LIBRARY)

# Handle the QUIETLY and REQUIRED arguments and set Elf_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Elf DEFAULT_MSG
  Elf_LIBRARY Elf_INCLUDE_DIR)

if(Elf_FOUND AND NOT TARGET Elf::elf)
  add_library(Elf::elf INTERFACE IMPORTED)
  set_property(TARGET Elf::elf
    PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Elf_INCLUDE_DIR})
  set_property(TARGET Elf::elf
    PROPERTY INTERFACE_LINK_LIBRARIES ${Elf_LIBRARY})
endif()

