#include <gtest/gtest.h>

#include <holydbg/arch_services.hpp>
#include <holydbg/binfmt/from_image.hpp>
#include <holydbg/dbg/debug_process.hpp>
#include <holydbg/dbg/debug_thread.hpp>
#include <holydbg/dbg/local/debuggee.hpp>
#include <holydbg/dbg/bps/sw_breakpoint.hpp>

#include <cassert>
#include <cstdint>

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

TEST(DebuggeeTest, DbgExecRun) {
  hdbg::ExecParams ep;
  ep.file = "dummy_exec";
  auto debuggee = hdbg::dbg_exec(ep);
  ASSERT_NO_THROW(debuggee->run());
}

TEST(DebuggeeTest, SetSwBpx) {
  hdbg::ExecParams ep;
  ep.file = "dummy_exec";
  auto debuggee = hdbg::dbg_exec(ep);
  
  auto& dbg_proc = debuggee->process();
  const auto entry_pt = process_entry_point(dbg_proc);
  const hdbg::breakpoint_id bpx_id = debuggee->set_bp(new hdbg::SwBreakpoint(entry_pt),
    [&bpx_id, entry_pt](hdbg::Debuggee & debuggee, hdbg::DebugThread & dbg_thr,
                        hdbg::ThreadContext & thr_ctx, hdbg::breakpoint_id bp_id)
    {
      EXPECT_EQ(bpx_id, bp_id);
      auto& dbg_proc = debuggee.process();
      EXPECT_EQ(&dbg_proc, &dbg_thr.process());
      auto& arch_svc = process_arch_services(dbg_proc);
      const auto ip_idx = arch_svc.reg_index("inst-ptr");
      const auto ip_at = thr_ctx.reg_value<std::uintptr_t>(ip_idx);
      EXPECT_EQ(entry_pt, ip_at);
    });
  
  debuggee->run();
}
