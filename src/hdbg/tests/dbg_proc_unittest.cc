#include <gtest/gtest.h>

#include <hdbg/this_process.hpp>
#include <hdbg/dbg/local/debug_process.hpp>

#include <cstdlib>
#include <cstdint>

class DbgProcTest
  : public ::testing::Test
{
public:
  DbgProcTest();
  virtual ~DbgProcTest();
  
  hdbg::LocalDebugProcess self_;
};

DbgProcTest::DbgProcTest()
  : self_ ( hdbg::this_process::id(),
              hdbg::DebugProcess::OpenFlags::MemRead  |
              hdbg::DebugProcess::OpenFlags::MemWrite ) {}

DbgProcTest::~DbgProcTest() = default;

TEST_F(DbgProcTest, SelfReadMemory) {
  int rd_var = std::rand();
  int rd_var_out;
  self_.read_mem(reinterpret_cast<std::uintptr_t>(&rd_var), sizeof(rd_var), &rd_var_out);
  ASSERT_EQ(rd_var, rd_var_out);
}

TEST_F(DbgProcTest, SelfWriteMemory) {
  int wr_var = std::rand();
  const int wr_var_in = ~wr_var;
  self_.write_mem(reinterpret_cast<std::uintptr_t>(&wr_var), sizeof(wr_var), &wr_var_in);
  ASSERT_EQ(wr_var, wr_var_in);
}

