set(SOURCES
  Atx.cpp
  Main.cpp
  Option.cpp
  Prstat.cpp
  Printing.cpp
  Generate.cpp
  Statistic.cpp
  Filtering.cpp
)
set(HEADERS
  Atx.hpp
  Option.hpp
  Prstat.hpp
  Printing.hpp
  Generate.hpp
  Statistic.hpp
  Filtering.hpp
)

add_executable(iata ${SOURCES} ${HEADERS})
target_include_directories(iata PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(iata PUBLIC iaisa iaelf iakrn iamac iaecu Elf::elf)

install(TARGETS iata
    RUNTIME DESTINATION bin
)
