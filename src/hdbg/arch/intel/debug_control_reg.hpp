#ifndef __HDBG_SRC_ARCH_INTEL_DEBUG_CONTROL_REG_HPP__
#define __HDBG_SRC_ARCH_INTEL_DEBUG_CONTROL_REG_HPP__

#include <cstdint>

namespace hdbg {

class DebugControlReg
{
public:
  enum break_on {
    break_on_exec  = 0,
    break_on_read  = 1,
    break_on_write = 2,
    break_on_read_or_write = 4,
  };
  
  enum bp_size {
    bp_size_byte  = 0,
    bp_size_word  = 1,
    bp_size_qword = 2,
    bp_size_dword = 3,
  };
  
  explicit constexpr DebugControlReg(std::uint32_t reg_value);
  
  void local_set_hw_bp_enabled(unsigned int dreg_offs, bool enable);
  bool is_local_hw_bp_enabled(unsigned int dreg_offs) const;
  
  void global_set_hw_bp_enabled(unsigned int dreg_offs, bool enable);
  bool is_global_hw_bp_enabled(unsigned int dreg_offs) const;
  
  void set_hw_bp_break_on(unsigned int dreg_offs, break_on brkon);
  void set_hw_bp_size(unsigned int dreg_offs, bp_size size);
  
  std::uint32_t value() const;
  
private:
  std::uint32_t value_;
};

constexpr DebugControlReg::DebugControlReg(std::uint32_t reg_value)
  : value_( reg_value ) {}

inline
std::uint32_t DebugControlReg::value() const
{
  return value_;
}

} // namespace hdbg

#endif // __HDBG_SRC_ARCH_INTEL_DEBUG_CONTROL_REG_HPP__
