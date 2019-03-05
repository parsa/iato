set(SOURCES
  KrnSig.cpp
  Syscall.cpp
  SysArgs.cpp
  KrnUser.cpp
  KrnProc.cpp
  KrnExit.cpp
  KrnMmap.cpp
  KrnTime.cpp
  KrnIoctl.cpp
  KrnFcntl.cpp
  KrnUname.cpp
)
set(HEADERS
  KrnErrno.hxx
  KrnExit.hpp
  KrnFcntl.hpp
  KrnFcntl.hxx
  KrnIoctl.hpp
  KrnIoctl.hxx
  KrnMmap.hpp
  KrnMmap.hxx
  KrnProc.hpp
  KrnSig.hpp
  KrnSig.hxx
  KrnTime.hpp
  KrnTime.hxx
  KrnTypes.hxx
  KrnUname.hpp
  KrnUname.hxx
  KrnUser.hpp
  KrnUser.hxx
  SysArgs.hpp
  Syscall.hpp
  Syscall.hxx
)

add_library(iakrn ${SOURCES} ${HEADERS})
target_include_directories(iakrn PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(iakrn PUBLIC $<$<CONFIG:Debug>:DEBUG> IATO OS_LINUX _REENTRANT HAS_PTHREAD)
target_link_libraries(iakrn PUBLIC iaisa iaelf Elf::elf)

