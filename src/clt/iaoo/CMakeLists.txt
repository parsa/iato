set(SOURCES
  Stx.cpp
  Main.cpp
  Option.cpp
  DlyStg.cpp
  ResStg.cpp
  IpgStg.cpp
  DecStg.cpp
  LrnStg.cpp
  PrnStg.cpp
  SlcStg.cpp
  RegStg.cpp
  ExeStg.cpp
  WbkStg.cpp
  CmtStg.cpp
  Resteer.cpp
  RseLogic.cpp
  Processor.cpp
  Simulator.cpp
)
set(HEADERS
  Stx.hpp
  Option.hpp
  DlyStg.hpp
  ResStg.hpp
  IpgStg.hpp
  DecStg.hpp
  LrnStg.hpp
  PrnStg.hpp
  SlcStg.hpp
  RegStg.hpp
  ExeStg.hpp
  WbkStg.hpp
  CmtStg.hpp
  Resteer.hpp
  RseLogic.hpp
  Processor.hpp
  Simulator.hpp
  Soo.hpp
  Srn.hpp
)

add_executable(iaoo ${SOURCES} ${HEADERS})
target_include_directories(iaoo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} /usr/include/libelf)
target_compile_definitions(iaoo PUBLIC $<$<CONFIG:Debug>:DEBUG> IATO OS_LINUX _REENTRANT HAS_PTHREAD)
target_link_libraries(iaoo PUBLIC iaisa iaelf iakrn iamac iaecu Elf::elf)

