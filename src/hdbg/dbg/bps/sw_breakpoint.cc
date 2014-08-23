#include <hdbg/dbg/bps/sw_breakpoint.hpp>

#include <hdbg/arch_services.hpp>
#include <hdbg/binfmt/from_image.hpp>
#include <hdbg/dbg/debuggee.hpp>
#include <hdbg/dbg/debug_process.hpp>
#include <hdbg/dbg/debug_thread.hpp>
#include <hdbg/dbg/thread_context.hpp>

#include "../../arch/arch_internals.hpp"

#include <cassert>
#include <cstdint>
#include <stdexcept>

namespace hdbg {

namespace {

ArchServices & process_arch_services(const DebugProcess & dbg_proc)
{
  const auto proc_img = binfmt_from_image(dbg_proc, dbg_proc.image_base());
  const auto proc_arch = proc_img->arch();
  return get_arch_services(proc_arch);
}

} // namespace

SwBreakpoint::SwBreakpoint(std::uintptr_t addr) : addr_( addr ) {}
SwBreakpoint::~SwBreakpoint() = default;

void SwBreakpoint::setup(Debuggee & debuggee)
{
  auto& dbg_proc = debuggee.process();
  const auto& arch_internals = process_arch_services(dbg_proc).get_internals();
  const auto& sw_bpx = arch_internals.sw_bpx_template();
  
  ov_data_.resize(sw_bpx.size());
  dbg_proc.read_mem(addr_, ov_data_.size(), ov_data_.data());
  dbg_proc.write_mem(addr_, sw_bpx.size(), sw_bpx.data());
}

void SwBreakpoint::cleanup(Debuggee & debuggee)
{
  auto& dbg_proc = debuggee.process();
  dbg_proc.write_mem(addr_, ov_data_.size(), ov_data_.data());
}

bool SwBreakpoint::match(const DebugThread & dbg_thr, const DebugEvent & dbg_evt,
                         const ThreadContext & thr_ctx) const
{
  if(dbg_evt.which() != UnknownEventId       &&
     dbg_evt.which() != BreakpointHitEventId )
  {
    return false;
  }
  
  auto& arch_svc = process_arch_services( dbg_thr.process() );
  const auto iptr = arch_svc.get_inst_ptr(thr_ctx);
  return iptr - ov_data_.size() == addr_;
}

void SwBreakpoint::rewind(DebugThread & dbg_thr, ThreadContext & thr_ctx) const
{
  auto& arch_svc = process_arch_services( dbg_thr.process() );
  const auto iptr = arch_svc.get_inst_ptr(thr_ctx);
  arch_svc.set_inst_ptr(thr_ctx, iptr - ov_data_.size());
  dbg_thr.set_context(thr_ctx);
}

} // namespace hdbg
