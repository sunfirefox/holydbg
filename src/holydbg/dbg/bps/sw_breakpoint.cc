#include <holydbg/dbg/bps/sw_breakpoint.hpp>

#include <holydbg/arch_services.hpp>
#include <holydbg/binfmt/from_image.hpp>
#include <holydbg/dbg/debuggee.hpp>
#include <holydbg/dbg/debug_process.hpp>
#include <holydbg/dbg/debug_thread.hpp>

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
  auto& arch_internl = process_arch_services(dbg_proc).get_internals();
  auto& arch_bp = arch_internl.make_sw_bp();
  ov_data_.resize(arch_bp.size());
  
  dbg_proc.read_mem(addr_, ov_data_.size(), ov_data_.data());
  dbg_proc.write_mem(addr_, arch_bp.size(), arch_bp.data());
}

void SwBreakpoint::cleanup(Debuggee & debuggee)
{
  auto& dbg_proc = debuggee.process();
  dbg_proc.write_mem(addr_, ov_data_.size(), ov_data_.data());
}

bool SwBreakpoint::match(const DebugThread & dbg_thr, const ThreadContext & thr_ctx) const
{
  // TODO: ip points before or after int3 instruction ???
  //  - linux: after
  //  - windows: ??
  auto& dbg_proc = dbg_thr.process();
  auto& arch_svc = process_arch_services(dbg_proc);
  const auto ip_idx = arch_svc.reg_index("inst-ptr");
  const auto thr_ip = thr_ctx.reg_value<std::uintptr_t>(ip_idx);
  return thr_ip - ov_data_.size() == addr_;
}

void SwBreakpoint::rewind(DebugThread & dbg_thr, ThreadContext & thr_ctx) const
{
  auto& dbg_proc = dbg_thr.process();
  auto& arch_svc = process_arch_services(dbg_proc);
  const int ip_idx = arch_svc.reg_index("inst-ptr");
  const auto ip_at = thr_ctx.reg_value<std::uintptr_t>(ip_idx);
  thr_ctx.set_reg(ip_idx, ip_at - ov_data_.size());
  dbg_thr.set_context(thr_ctx);
}

} // namespace hdbg
