#include <gtest/gtest.h>

#include <hdbg/dbg/debug_process.hpp>
#include <hdbg/dbg/debug_thread.hpp>
#include <hdbg/dbg/thread_context.hpp>
#include <hdbg/dbg/local/debuggee.hpp>
#include <hdbg/enum/enum_threads.hpp>

#include <cstdint>
#include <memory>

class ThreadContextTest
  : public ::testing::Test
{
public:
  ThreadContextTest();
  virtual ~ThreadContextTest();
  
  std::unique_ptr<hdbg::Debuggee> debuggee_;
  hdbg::DebugThread * dbg_thr_ = nullptr;
  hdbg::ThreadContext thr_ctx_;
};

ThreadContextTest::ThreadContextTest()
  : debuggee_([]{
      hdbg::DbgExecParams ep;
      ep.file = "dummy_exec";
      return hdbg::LocalDebuggee::exec(ep);
    }())
  , dbg_thr_([this]{
      const auto& dbg_proc = debuggee_->process();
      const auto thr_itr = hdbg::enum_threads(dbg_proc.id()).begin();
      return &debuggee_->get_thread(thr_itr->tid);
    }())
{
  dbg_thr_->get_context(thr_ctx_);
  thr_ctx_.set_reg(0, 0xdeadbeef);
}

ThreadContextTest::~ThreadContextTest()
{
  debuggee_->kill();
}

TEST_F(ThreadContextTest, BasicRegValue) {
  const hdbg::RegValue rv = thr_ctx_.reg_value(0);
  ASSERT_EQ(0xdeadbeef, rv);
}

TEST_F(ThreadContextTest, CopyOnWrite) {
  hdbg::ThreadContext ctx_copy ( thr_ctx_ );
  EXPECT_EQ(0xdeadbeef, ctx_copy.reg_value(0));
  ctx_copy.set_reg(0, 0xcafebabe);
  EXPECT_EQ(0xcafebabe, ctx_copy.reg_value(0));
  EXPECT_EQ(0xdeadbeef, thr_ctx_.reg_value(0));
}

