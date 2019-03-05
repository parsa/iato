set(SOURCES
  Etx.cpp
  ElfBsa.cpp
  ElfBrk.cpp
  ElfMap.cpp
  ElfText.cpp
  ElfLoad.cpp
  ElfExec.cpp
  ElfArgs.cpp
  ElfEnvp.cpp
  ElfStack.cpp
  ElfTable.cpp
  ElfImage.cpp
  ElfKernel.cpp
  ElfInterp.cpp
  ElfMemory.cpp
  ElfSection.cpp
  ElfSegment.cpp
  ElfChecker.cpp
)
set(HEADERS
  Abi.hpp
  Elf.hxx
  ElfArgs.hpp
  ElfBrk.hpp
  ElfBsa.hpp
  ElfChecker.hpp
  ElfEnvp.hpp
  ElfExec.hpp
  ElfImage.hpp
  ElfInterp.hpp
  ElfInterp.hxx
  ElfKernel.hpp
  ElfLoad.hpp
  ElfMap.hpp
  ElfMemory.hpp
  ElfSection.hpp
  ElfSegment.hpp
  ElfStack.hpp
  ElfTable.hpp
  ElfText.hpp
  Etx.hpp
)

add_library(iaelf ${SOURCES} ${HEADERS})
target_include_directories(iaelf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(iaelf PUBLIC $<$<CONFIG:Debug>:DEBUG> IATO OS_LINUX _REENTRANT HAS_PTHREAD)
target_link_libraries(iaelf PUBLIC iaisa Elf::elf)

