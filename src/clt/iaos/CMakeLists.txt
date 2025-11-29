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
target_link_libraries(iaos PUBLIC iaisa iaelf iakrn iamac iaecu Elf::elf)

install(TARGETS iaos
    RUNTIME DESTINATION bin
)
