set(hdbg_tests
      events
      endian
      dbg_proc
      enum_proc
      enum_thr
      binfmt_img
      local_debuggee
      thr_ctx)

foreach(test ${hdbg_tests})
  add_executable(${test}_unittest ${test}_unittest.cc)
  target_link_libraries(${test}_unittest gtest gtest_main hdbg)
  add_test(NAME ${test} COMMAND ${test}_unittest)
endforeach()

add_executable(dummy_exec dummy_exec.cc)
