set(SOURCES
  Stx.cpp
  Main.cpp
  Option.cpp
  IpgStg.cpp
  RotStg.cpp
  ExpStg.cpp
  RenStg.cpp
  DlyStg.cpp
  ResStg.cpp
  RegStg.cpp
  ExeStg.cpp
  DetStg.cpp
  Fp1Stg.cpp
  Fp2Stg.cpp
  Fp3Stg.cpp
  Fp4Stg.cpp
  BobStg.cpp
  BedStg.cpp
  WrbStg.cpp
  FwbStg.cpp
  Resteer.cpp
  RseLogic.cpp
  Processor.cpp
  Simulator.cpp
)
set(HEADERS
  Stx.hpp
  Option.hpp
  IpgStg.hpp
  RotStg.hpp
  ExpStg.hpp
  RenStg.hpp
  DlyStg.hpp
  ResStg.hpp
  RegStg.hpp
  ExeStg.hpp
  DetStg.hpp
  Fp1Stg.hpp
  Fp2Stg.hpp
  Fp3Stg.hpp
  Fp4Stg.hpp
  BobStg.hpp
  BedStg.hpp
  WrbStg.hpp
  FwbStg.hpp
  Resteer.hpp
  RseLogic.hpp
  Processor.hpp
  Simulator.hpp
  Sio.hpp
  Srn.hpp
)

add_executable(iaio ${SOURCES} ${HEADERS})
target_include_directories(iaio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(iaio PUBLIC $<$<CONFIG:Debug>:DEBUG> IATO OS_LINUX _REENTRANT HAS_PTHREAD)
target_link_libraries(iaio PUBLIC iaisa iaelf iakrn iamac iaecu Elf::elf)

