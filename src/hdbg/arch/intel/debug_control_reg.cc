#include "debug_control_reg.hpp"

#include <cassert>

namespace hdbg {

namespace {

inline constexpr bool is_valid_dreg_offs(unsigned int dreg_offs)
{
  return dreg_offs < 4;
}

} // namespace

void DebugControlReg::local_set_hw_bp_enabled(unsigned int dreg_offs, bool enable)
{
  assert( is_valid_dreg_offs(dreg_offs) );
  const std::uint32_t dreg_shift = dreg_offs * 2; // 0, 2, 4, 6
  const std::uint32_t dreg_mask = ~(1 << dreg_shift);
  value_ = (value_ & dreg_mask) | enable << dreg_shift;
}

bool DebugControlReg::is_local_hw_bp_enabled(unsigned int dreg_offs) const
{
  assert( is_valid_dreg_offs(dreg_offs) );
  const std::uint32_t dreg_shift = dreg_offs * 2; // 0, 2, 4, 6
  const std::uint32_t dreg_mask = ~(1 << dreg_shift);
  return value_ & dreg_mask;
}

void DebugControlReg::global_set_hw_bp_enabled(unsigned int dreg_offs, bool enable)
{
  assert( is_valid_dreg_offs(dreg_offs) );
  const std::uint32_t dreg_shift = 1 + dreg_offs * 2; // 1, 3, 5, 7
  const std::uint32_t dreg_mask = ~(1 << dreg_shift);
  value_ = (value_ & dreg_mask) | enable << dreg_shift;
}

bool DebugControlReg::is_global_hw_bp_enabled(unsigned int dreg_offs) const
{
  assert( is_valid_dreg_offs(dreg_offs) );
  const std::uint32_t dreg_shift = 1 + dreg_offs * 2; // 1, 3, 5, 7
  const std::uint32_t dreg_mask = ~(1 << dreg_shift);
  return value_ & dreg_mask;
}

void DebugControlReg::set_hw_bp_break_on(unsigned int dreg_offs, break_on brkon)
{
  assert( is_valid_dreg_offs(dreg_offs) );
  const std::uint32_t dreg_shift = 16 + dreg_offs * 4; // 16, 20, 24, 28
  const std::uint32_t dreg_mask = ~(3 << dreg_shift);
  value_ = (value_ & dreg_mask) | brkon << dreg_shift;
}

void DebugControlReg::set_hw_bp_size(unsigned int dreg_offs, bp_size size)
{
  assert( is_valid_dreg_offs(dreg_offs) );
  const std::uint32_t dreg_shift = 18 + dreg_offs * 4; // 18, 22, 26, 30
  const std::uint32_t dreg_mask = ~(3 << dreg_shift);
  value_ = (value_ & dreg_mask) | size << dreg_shift;
}

} // namespace hdbg
