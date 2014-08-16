#ifndef __HDBG_SRC_ARCH_INTEL_X86_64_ARCH_INTERNALS_HPP__
#define __HDBG_SRC_ARCH_INTEL_X86_64_ARCH_INTERNALS_HPP__

#include "../../arch_internals.hpp"

namespace hdbg {

class X64_ArchInternals final : public ArchInternals
{
public:
  X64_ArchInternals();
  virtual ~X64_ArchInternals();
  
  virtual std::unique_ptr<RawContext> make_raw_context() const override;
  virtual const std::vector<std::uint8_t> & sw_bpx_template() const override;
  virtual const std::vector<unsigned int> & hw_bpx_reg_indexes() const override;
  virtual bool is_hw_bpx_reg_enabled(unsigned int reg_idx, const ThreadContext & thr_ctx) const override;
  virtual void set_hw_bpx_enabled(unsigned int reg_idx, bool enabled, ThreadContext & thr_ctx) const override;
};

} // namespace hdbg

#endif // __HDBG_SRC_ARCH_INTEL_X86_64_ARCH_INTERNALS_HPP__
