#include <holydbg/dbg/bps/hw_breakpoint.hpp>

#include <holydbg/arch_services.hpp>
#include <holydbg/binfmt/from_image.hpp>
#include <holydbg/dbg/debuggee.hpp>
#include <holydbg/dbg/debug_process.hpp>
#include <holydbg/dbg/debug_thread.hpp>
#include <holydbg/dbg/thread_context.hpp>
#include <holydbg/enum/enum_threads.hpp>

#include "../../arch/arch_internals.hpp"

#include <algorithm>

namespace hdbg {

namespace {

ArchServices & process_arch_services(const DebugProcess & dbg_proc)
{
  const auto proc_img = binfmt_from_image(dbg_proc, dbg_proc.image_base());
  assert( proc_img );
  const auto proc_arch = proc_img->arch();
  assert( proc_arch );
  return get_arch_services(proc_arch);
}

} // namespace

HwBreakpoint::HwBpxDbgEvtListener::HwBpxDbgEvtListener(HwBreakpoint & hw_bpx)
  : hw_bpx_(hw_bpx) {}

void HwBreakpoint::HwBpxDbgEvtListener::handle_event(const ThreadCreatedEvent & dbg_evt)
{
  hw_bpx_.set_on_thread(*dbg_evt.new_thr);
}

void HwBreakpoint::HwBpxDbgEvtListener::handle_event(const ThreadExitedEvent & dbg_evt)
{
  hw_bpx_.remove_from_thread(*dbg_evt.thr);
}

HwBreakpoint::HwBreakpoint(std::uintptr_t addr)
  : addr_( addr )
  , dbgevt_listener_( std::make_shared<HwBpxDbgEvtListener>(*this) ) {}

HwBreakpoint::~HwBreakpoint() = default;

void HwBreakpoint::setup(Debuggee & debuggee)
{
  auto& dbg_proc = debuggee.process();
  for(const auto& thr_e : enum_threads(dbg_proc.id()))
    set_on_thread( debuggee.get_thread(thr_e.tid) );
}

void HwBreakpoint::cleanup(Debuggee & debuggee)
{
  auto& dbg_proc = debuggee.process();
  for(const auto& thr_e : enum_threads(dbg_proc.id()))
    remove_from_thread( debuggee.get_thread(thr_e.tid) );
}

bool HwBreakpoint::match(const DebugThread & dbg_thr, const ThreadContext & thr_ctx) const
{
  auto& arch_svc = process_arch_services( dbg_thr.process() );
  const auto ip_idx = arch_svc.reg_index("inst-ptr");
  return thr_ctx.reg_value<std::uintptr_t>(ip_idx) == addr_;
}

void HwBreakpoint::rewind(DebugThread & dbg_thr, ThreadContext & thr_ctx) const {}

void HwBreakpoint::set_on_thread(DebugThread & dbg_thr)
{
  auto& arch_svc = process_arch_services( dbg_thr.process() );
  auto& arch_internals = arch_svc.get_internals();
  const auto& hw_bpx_regidx = arch_internals.hw_bpx_reg_indexes();
  dbg_thr.get_context(thr_ctx_);
  for(auto reg_idx : hw_bpx_regidx) {
    if(!arch_internals.is_hw_bpx_reg_enabled(reg_idx, thr_ctx_)) {
      thr_ctx_.set_reg(reg_idx, addr_);
      arch_internals.set_hw_bpx_enabled(reg_idx, true, thr_ctx_);
      dbg_thr.set_context(thr_ctx_);
      bp_regs_[ dbg_thr.id() ] = reg_idx;
      break;
    }
  }
}

void HwBreakpoint::remove_from_thread(DebugThread & dbg_thr)
{
  auto bpr_itr = bp_regs_.find(dbg_thr.id());
  if(bpr_itr != bp_regs_.end()) {
    auto& arch_svc = process_arch_services( dbg_thr.process() );
    auto& arch_internals = arch_svc.get_internals();
    dbg_thr.get_context(thr_ctx_);
    thr_ctx_.set_reg(bpr_itr->second, 0);
    arch_internals.set_hw_bpx_enabled(bpr_itr->second, false, thr_ctx_);
    dbg_thr.set_context(thr_ctx_);
    bp_regs_.erase(bpr_itr);
  }
}

} // namespace hdbg
