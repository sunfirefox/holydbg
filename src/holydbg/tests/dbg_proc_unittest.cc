#include <gtest/gtest.h>

#include <holydbg/this_process.hpp>
#include <holydbg/dbg/local/debug_process.hpp>

#include <cstdlib>

hdbg::LocalDebugProcess self ( hdbg::this_process::id(),
                               hdbg::DebugProcess::OpenFlags::MemRead  |
                               hdbg::DebugProcess::OpenFlags::MemWrite );

TEST(DbgProcTest, SelfReadMemory) {
  int rd_var = std::rand();
  int rd_var_out;
  self.read_mem(reinterpret_cast<std::uintptr_t>(&rd_var), sizeof(rd_var), &rd_var_out);
  ASSERT_EQ(rd_var, rd_var_out);
}

TEST(DbgProcTest, SelfWriteMemory) {
  int wr_var = std::rand();
  const int wr_var_in = ~wr_var;
  self.write_mem(reinterpret_cast<std::uintptr_t>(&wr_var), sizeof(wr_var), &wr_var_in);
  ASSERT_EQ(wr_var, wr_var_in);
}

