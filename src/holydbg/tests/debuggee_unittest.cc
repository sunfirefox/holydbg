#include <gtest/gtest.h>

#include <holydbg/arch_services.hpp>
#include <holydbg/binfmt/from_image.hpp>
#include <holydbg/dbg/debug_process.hpp>
#include <holydbg/dbg/debug_thread.hpp>
#include <holydbg/dbg/local/debuggee.hpp>
#include <holydbg/dbg/bps/sw_breakpoint.hpp>
#include <holydbg/dbg/bps/hw_breakpoint.hpp>

#include <cassert>
#include <cstdint>
#include <memory>

namespace {

hdbg::ArchServices & process_arch_services(const hdbg::DebugProcess & dbg_proc)
{
  const auto proc_img = hdbg::binfmt_from_image(dbg_proc, dbg_proc.image_base());
  assert( proc_img );
  const auto proc_arch = proc_img->arch();
  assert( proc_arch );
  return hdbg::get_arch_services(proc_arch);
}

std::uintptr_t process_entry_point(const hdbg::DebugProcess & dbg_proc)
{
  const auto img_base = dbg_proc.image_base();
  const auto proc_img = hdbg::binfmt_from_image(dbg_proc, img_base);
  assert( proc_img );
  return proc_img->entry();
}

} // namespace

class DebuggeeTest
  : public ::testing::Test
{
public:
  DebuggeeTest();
  virtual ~DebuggeeTest();
  
  std::unique_ptr<hdbg::Debuggee> debuggee_;
  hdbg::DebugProcess & dbg_proc_;
  std::uintptr_t entry_pt_;
  hdbg::ArchServices & arch_svc_;
  unsigned int ip_idx_;
};

DebuggeeTest::DebuggeeTest()
  : debuggee_([]{
      hdbg::ExecParams ep;
      ep.file = "dummy_exec";
      return hdbg::dbg_exec(ep);
    }())
  , dbg_proc_( debuggee_->process() )
  , entry_pt_( process_entry_point(dbg_proc_) )
  , arch_svc_( process_arch_services(dbg_proc_) )
  , ip_idx_ ( arch_svc_.reg_index("inst-ptr") ) {}

DebuggeeTest::~DebuggeeTest() = default;

TEST_F(DebuggeeTest, DbgExecRun) {
  ASSERT_NO_THROW(debuggee_->run());
}

TEST_F(DebuggeeTest, SetSwBpx) {
  bool bp_hit = false;
  const hdbg::breakpoint_id bpx_id = debuggee_->set_bp(new hdbg::SwBreakpoint(entry_pt_),
    [this, &bpx_id, &bp_hit](hdbg::Debuggee & debuggee, hdbg::DebugThread & dbg_thr,
                             hdbg::ThreadContext & thr_ctx, hdbg::breakpoint_id bp_id)
    {
      EXPECT_EQ(bpx_id, bp_id);
      auto& dbg_proc = debuggee.process();
      EXPECT_EQ(&dbg_proc, &dbg_proc_);
      const auto ip_at = thr_ctx.reg_value<std::uintptr_t>(ip_idx_);
      EXPECT_EQ(entry_pt_, ip_at);
      bp_hit = true;
    });
  
  debuggee_->run();
  ASSERT_TRUE(bp_hit);
}

TEST_F(DebuggeeTest, SetHwBpx) {
  bool bp_hit = false;
  const hdbg::breakpoint_id bpx_id = debuggee_->set_bp(new hdbg::HwBreakpoint(entry_pt_),
    [this, &bpx_id, &bp_hit](hdbg::Debuggee & debuggee, hdbg::DebugThread & dbg_thr,
                             hdbg::ThreadContext & thr_ctx, hdbg::breakpoint_id bp_id)
    {
      EXPECT_EQ(bpx_id, bp_id);
      auto& dbg_proc = debuggee.process();
      EXPECT_EQ(&dbg_proc, &dbg_proc_);
      const auto ip_at = thr_ctx.reg_value<std::uintptr_t>(ip_idx_);
      EXPECT_EQ(entry_pt_, ip_at);
      bp_hit = true;
    });
  
  debuggee_->run();
  ASSERT_TRUE(bp_hit);
}

