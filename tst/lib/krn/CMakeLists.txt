set(TEST_CASES
  t_open_flags.cpp
)

foreach(test_file ${TEST_CASES})
  get_filename_component(test_target ${test_file} NAME_WE)
  add_executable(${test_target} ${test_file})
  target_link_libraries(${test_target} PUBLIC iakrn)
  add_test(NAME tst.lib.krn.${test_target} COMMAND ${test_target})
endforeach()


