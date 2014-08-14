#include <gtest/gtest.h>

#include <holydbg/this_process.hpp>
#include <holydbg/enum/enum_processes.hpp>

TEST(EnumProcTest, FindSelfProc) {
  const hdbg::process_id self_pid = hdbg::this_process::id();
  bool found = false;
  for(const auto& proc_e : hdbg::enum_processes())
    if(proc_e.pid == self_pid)
      found = true;
  ASSERT_TRUE(found);
}

