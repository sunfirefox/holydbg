#include "arch_internals.hpp"

#include <hdbg/dbg/thread_context.hpp>

#include "reg_ids.hpp"
#include "raw_context.hpp"
#include "../debug_control_reg.hpp"

#include <stdexcept>

namespace hdbg {

namespace {

int reg_idx_to_dreg_offs(unsigned int reg_idx)
{
  const int dreg_offs = reg_idx - X64_RegDr0;
  if(dreg_offs < 0 || dreg_offs > 3)
    throw std::invalid_argument("invalid reg index");
  return dreg_offs;
}

} // namespace

X64_ArchInternals::X64_ArchInternals() = default;
X64_ArchInternals::~X64_ArchInternals() = default;

std::shared_ptr<RawContext> X64_ArchInternals::make_raw_context() const
{
  return std::make_shared<X64_RawContext>();
}

const std::vector<std::uint8_t>& X64_ArchInternals::sw_bpx_template() const
{
  static const std::vector<std::uint8_t> sw_bpx { 0xcc }; // __asm int3
  return sw_bpx;
}

const std::vector<unsigned int> & X64_ArchInternals::hw_bpx_reg_indexes() const
{
  static const std::vector<unsigned int> hw_bpx_regidx = {
    X64_RegDr0, X64_RegDr1, X64_RegDr2, X64_RegDr3
  };
  return hw_bpx_regidx;
}

bool X64_ArchInternals::is_hw_bpx_reg_enabled(unsigned int reg_idx, const ThreadContext & thr_ctx) const
{
  const int dreg_offs = reg_idx_to_dreg_offs(reg_idx);
  const auto dr7_value = static_cast<std::uint64_t>( thr_ctx.reg_value(X64_RegDr7) );
  const X64_DebugControlReg dreg_ctrl( dr7_value );
  return dreg_ctrl.is_local_hw_bp_enabled(dreg_offs);
}

void X64_ArchInternals::set_hw_bpx_enabled(unsigned int reg_idx, bool enabled, ThreadContext & thr_ctx) const
{
  const int dreg_offs = reg_idx_to_dreg_offs(reg_idx);
  const auto dr7_value = static_cast<std::uint64_t>( thr_ctx.reg_value(X64_RegDr7) );
  X64_DebugControlReg dreg_ctrl( dr7_value );
  dreg_ctrl.local_set_hw_bp_enabled(dreg_offs, enabled);
  if(enabled) {
    dreg_ctrl.set_hw_bp_break_on(dreg_offs, X64_DebugControlReg::BreakOnExec);
    dreg_ctrl.set_hw_bp_size(dreg_offs, X64_DebugControlReg::BpSizeByte);
  }
  thr_ctx.set_reg(X64_RegDr7, dreg_ctrl.value());
}

} // namespace hdbg
