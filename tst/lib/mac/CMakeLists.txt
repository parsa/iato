set(TEST_CASES
  t_cdirect.cpp
)

foreach(test_file ${TEST_CASES})
  get_filename_component(test_target ${test_file} NAME_WE)
  add_executable(${test_target} ${test_file})
  target_compile_definitions(${test_target} PUBLIC $<$<CONFIG:Debug>:DEBUG> IATO OS_LINUX _REENTRANT HAS_PTHREAD)
  target_link_libraries(${test_target} PUBLIC iamac Elf::elf)
endforeach()

