#include <hdbg/dbg/bps/sw_breakpoint.hpp>

#include <hdbg/arch_services.hpp>
#include <hdbg/binfmt/from_image.hpp>
#include <hdbg/dbg/debuggee.hpp>
#include <hdbg/dbg/debug_process.hpp>
#include <hdbg/dbg/debug_thread.hpp>

#include "../../arch/arch_internals.hpp"

#include <cassert>
#include <cstdint>
#include <stdexcept>

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

SwBreakpoint::SwBreakpoint(std::uintptr_t addr) : addr_( addr ) {}
SwBreakpoint::~SwBreakpoint() = default;

void SwBreakpoint::setup(Debuggee & debuggee)
{
  auto& dbg_proc = debuggee.process();
  auto& arch_internals = process_arch_services(dbg_proc).get_internals();
  auto& sw_bpx = arch_internals.sw_bpx_template();
  ov_data_.resize(sw_bpx.size());
  
  dbg_proc.read_mem(addr_, ov_data_.size(), ov_data_.data());
  dbg_proc.write_mem(addr_, sw_bpx.size(), sw_bpx.data());
}

void SwBreakpoint::cleanup(Debuggee & debuggee)
{
  auto& dbg_proc = debuggee.process();
  dbg_proc.write_mem(addr_, ov_data_.size(), ov_data_.data());
}

bool SwBreakpoint::match(const DebugThread & dbg_thr, const ThreadContext & thr_ctx) const
{
  auto& arch_svc = process_arch_services( dbg_thr.process() );
  const auto ip_idx = arch_svc.reg_index("inst-ptr");
  const auto thr_ip = thr_ctx.reg_value<std::uintptr_t>(ip_idx);
  return thr_ip - ov_data_.size() == addr_;
}

void SwBreakpoint::rewind(DebugThread & dbg_thr, ThreadContext & thr_ctx) const
{
  auto& arch_svc = process_arch_services( dbg_thr.process() );
  const int ip_idx = arch_svc.reg_index("inst-ptr");
  const auto ip_at = thr_ctx.reg_value<std::uintptr_t>(ip_idx);
  thr_ctx.set_reg(ip_idx, ip_at - ov_data_.size());
  dbg_thr.set_context(thr_ctx);
}

} // namespace hdbg
