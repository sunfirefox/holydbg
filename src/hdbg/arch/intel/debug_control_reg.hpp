#ifndef __HDBG_SRC_ARCH_INTEL_DEBUG_CONTROL_REG_HPP__
#define __HDBG_SRC_ARCH_INTEL_DEBUG_CONTROL_REG_HPP__

#include <cstdint>

namespace hdbg {

template <typename T>
class DebugControlReg
{
public:
  enum BreakOn {
    BreakOnExec  = 0,
    BreakOnRead  = 1,
    BreakOnWrite = 2,
    BreakOnReadOrWrite = 4,
  };
  
  enum BpSize {
    BpSizeByte  = 0,
    BpSizeWord  = 1,
    BpSizeQword = 2,
    BpSizeDword = 3,
  };
  
  explicit constexpr DebugControlReg(T reg_value);
  
  void local_set_hw_bp_enabled(int dreg_offs, bool enable);
  bool is_local_hw_bp_enabled(int dreg_offs) const;
  
  void global_set_hw_bp_enabled(int dreg_offs, bool enable);
  bool is_global_hw_bp_enabled(int dreg_offs) const;
  
  void set_hw_bp_break_on(int dreg_offs, BreakOn brkon);
  void set_hw_bp_size(int dreg_offs, BpSize size);
  
  T value() const;
  
private:
  T value_;
};

typedef DebugControlReg<std::uint32_t> X86_DebugControlReg;
typedef DebugControlReg<std::uint64_t> X64_DebugControlReg;

namespace detail {

inline bool is_valid_dreg_offs(int dreg_offs)
{
  return dreg_offs >= 0 && dreg_offs < 4;
}

} // namespace detail

template <typename T>
constexpr DebugControlReg<T>::DebugControlReg(T reg_value)
  : value_( reg_value ) {}

template <typename T>
void DebugControlReg<T>::local_set_hw_bp_enabled(int dreg_offs, bool enable)
{
  assert( detail::is_valid_dreg_offs(dreg_offs) );
  const T dreg_shift = dreg_offs * 2; // 0, 2, 4, 6
  const T dreg_mask = ~(1 << dreg_shift);
  value_ = (value_ & dreg_mask) | enable << dreg_shift;
}

template <typename T>
bool DebugControlReg<T>::is_local_hw_bp_enabled(int dreg_offs) const
{
  assert( detail::is_valid_dreg_offs(dreg_offs) );
  const T dreg_shift = dreg_offs * 2; // 0, 2, 4, 6
  const T dreg_mask = ~(1 << dreg_shift);
  return value_ & dreg_mask;
}

template <typename T>
void DebugControlReg<T>::global_set_hw_bp_enabled(int dreg_offs, bool enable)
{
  assert( detail::is_valid_dreg_offs(dreg_offs) );
  const T dreg_shift = 1 + dreg_offs * 2; // 1, 3, 5, 7
  const T dreg_mask = ~(1 << dreg_shift);
  value_ = (value_ & dreg_mask) | enable << dreg_shift;
}

template <typename T>
bool DebugControlReg<T>::is_global_hw_bp_enabled(int dreg_offs) const
{
  assert( detail::is_valid_dreg_offs(dreg_offs) );
  const T dreg_shift = 1 + dreg_offs * 2; // 1, 3, 5, 7
  const T dreg_mask = ~(1 << dreg_shift);
  return value_ & dreg_mask;
}

template <typename T>
void DebugControlReg<T>::set_hw_bp_break_on(int dreg_offs, BreakOn brkon)
{
  assert( detail::is_valid_dreg_offs(dreg_offs) );
  const T dreg_shift = 16 + dreg_offs * 4; // 16, 20, 24, 28
  const T dreg_mask = ~(3 << dreg_shift);
  value_ = (value_ & dreg_mask) | brkon << dreg_shift;
}

template <typename T>
void DebugControlReg<T>::set_hw_bp_size(int dreg_offs, BpSize size)
{
  assert( detail::is_valid_dreg_offs(dreg_offs) );
  const T dreg_shift = 18 + dreg_offs * 4; // 18, 22, 26, 30
  const T dreg_mask = ~(3 << dreg_shift);
  value_ = (value_ & dreg_mask) | size << dreg_shift;
}

template <typename T>
T DebugControlReg<T>::value() const
{
  return value_;
}

} // namespace hdbg

#endif // __HDBG_SRC_ARCH_INTEL_DEBUG_CONTROL_REG_HPP__
