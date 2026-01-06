set(TEST_CASES
  t_ip.cpp
  t_rid.cpp
  t_cfm.cpp
  t_rse_spillfill.cpp
  t_uvr.cpp
  t_umr.cpp
  t_psr.cpp
  t_bits.cpp
  t_huge.cpp
  t_real.cpp
  t_alat.cpp
  t_fpsr.cpp
  t_bundle.cpp
)

foreach(test_file ${TEST_CASES})
  get_filename_component(test_target ${test_file} NAME_WE)
  add_executable(${test_target} ${test_file})
  target_link_libraries(${test_target} PUBLIC iaisa Elf::elf)
  add_test(NAME tst.lib.isa.${test_target} COMMAND ${test_target})
endforeach()

