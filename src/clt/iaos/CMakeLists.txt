set(SOURCES
  Main.cpp
  Option.cpp
  Binary.cpp
  Verify.cpp
  Disass.cpp
  Statistic.cpp
)
set(HEADERS
  Option.hpp
  Binary.hpp
  Verify.hpp
  Disass.hpp
  Statistic.hpp
)

add_executable(iaos ${SOURCES} ${HEADERS})
target_include_directories(iaos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(iaos PUBLIC $<$<CONFIG:Debug>:DEBUG> IATO OS_LINUX _REENTRANT HAS_PTHREAD)
target_link_libraries(iaos PUBLIC iaisa iaelf iakrn iamac iaecu Elf::elf)

