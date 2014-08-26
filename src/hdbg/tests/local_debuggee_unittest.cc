#include <gtest/gtest.h>

#include <hdbg/arch_services.hpp>
#include <hdbg/binfmt/from_image.hpp>
#include <hdbg/dbg/debug_process.hpp>
#include <hdbg/dbg/debug_thread.hpp>
#include <hdbg/dbg/local/debuggee.hpp>
#include <hdbg/dbg/bps/sw_breakpoint.hpp>
#include <hdbg/dbg/bps/hw_breakpoint.hpp>

#include <cassert>
#include <cstdint>
#include <memory>

class LocalDebuggeeTest
  : public ::testing::Test
{
public:
  LocalDebuggeeTest();
  virtual ~LocalDebuggeeTest();
  
  std::unique_ptr<hdbg::LocalDebuggee> debuggee_;
  hdbg::DebugProcess & dbg_proc_;
  std::uintptr_t entry_pt_;
  hdbg::ArchServices & arch_svc_;
};

namespace {

hdbg::ArchServices & process_arch_services(const hdbg::DebugProcess & dbg_proc)
{
  const auto proc_img = hdbg::binfmt_from_image(dbg_proc, dbg_proc.image_base());
  const auto proc_arch = proc_img->arch();
  return hdbg::get_arch_services(proc_arch);
}

std::uintptr_t process_entry_point(const hdbg::DebugProcess & dbg_proc)
{
  const auto img_base = dbg_proc.image_base();
  const auto proc_img = hdbg::binfmt_from_image(dbg_proc, img_base);
  return dbg_proc.image_base() + proc_img->entry();
}

} // namespace

LocalDebuggeeTest::LocalDebuggeeTest()
  : debuggee_([]{
      hdbg::ExecParams ep;
      ep.file = "dummy_exec";
      return hdbg::dbg_exec(ep);
    }())
  , dbg_proc_( debuggee_->process() )
  , entry_pt_( process_entry_point(dbg_proc_) )
  , arch_svc_( process_arch_services(dbg_proc_) ) {}

LocalDebuggeeTest::~LocalDebuggeeTest()
{
  assert( !debuggee_->attached() );
}

TEST_F(LocalDebuggeeTest, DbgExecRun) {
  ASSERT_NO_THROW(debuggee_->run());
}

TEST_F(LocalDebuggeeTest, SetSwBpx) {
  unsigned int bp_hits = 0;
  const hdbg::breakpoint_id bpx_id = debuggee_->set_bp(new hdbg::SwBreakpoint(entry_pt_),
    [this, &bpx_id, &bp_hits](hdbg::Debuggee & debuggee, hdbg::DebugThread & dbg_thr,
                              hdbg::ThreadContext thr_ctx, hdbg::breakpoint_id bp_id)
    {
      EXPECT_EQ(bpx_id, bp_id);
      auto& dbg_proc = debuggee.process();
      EXPECT_EQ(&dbg_proc, &dbg_proc_);
      const auto iptr = arch_svc_.get_inst_ptr(thr_ctx);
      EXPECT_EQ(entry_pt_, iptr);
      ++bp_hits;
    });
  debuggee_->run();
  ASSERT_EQ(1, bp_hits);
}

TEST_F(LocalDebuggeeTest, SetHwBpx) {
  unsigned int bp_hits = 0;
  const hdbg::breakpoint_id bpx_id = debuggee_->set_bp(new hdbg::HwBreakpoint(entry_pt_),
    [this, &bpx_id, &bp_hits](hdbg::Debuggee & debuggee, hdbg::DebugThread & dbg_thr,
                              hdbg::ThreadContext thr_ctx, hdbg::breakpoint_id bp_id)
    {
      EXPECT_EQ(bpx_id, bp_id);
      auto& dbg_proc = debuggee.process();
      EXPECT_EQ(&dbg_proc, &dbg_proc_);
      const auto iptr = arch_svc_.get_inst_ptr(thr_ctx);
      EXPECT_EQ(entry_pt_, iptr);
      ++bp_hits;
    });
  debuggee_->run();
  ASSERT_EQ(1, bp_hits);
}

