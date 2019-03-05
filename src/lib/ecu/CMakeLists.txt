set(SOURCES
  Utx.cpp
  Mapper.cpp
  Fetcher.cpp
  Renamer.cpp
)
set(HEADERS
  Ecu.hpp
  Fetcher.hpp
  Mapper.hpp
  Renamer.hpp
  Urn.hpp
  Utx.hpp
)

add_library(iaecu ${SOURCES} ${HEADERS})
target_include_directories(iaecu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(iaecu PUBLIC $<$<CONFIG:Debug>:DEBUG> IATO OS_LINUX _REENTRANT HAS_PTHREAD)
target_link_libraries(iaecu PUBLIC iaisa iaelf iakrn iamac Elf::elf)

