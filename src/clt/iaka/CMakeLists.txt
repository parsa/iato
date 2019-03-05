set(SOURCES
  Atx.cpp
  Main.cpp
  Option.cpp
  Emulator.cpp
)
set(HEADERS
  Atx.hpp
  Option.hpp
  Emulator.hpp
  Eio.hpp
  Ern.hpp
)

add_executable(iaka ${SOURCES} ${HEADERS})
target_include_directories(iaka PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(iaka PUBLIC $<$<CONFIG:Debug>:DEBUG> IATO OS_LINUX _REENTRANT HAS_PTHREAD)
target_link_libraries(iaka PUBLIC iaisa iaelf iakrn iamac iaecu Elf::elf)

