#ifndef __HOLYDBG_SRC_ARCH_X86_64_ARCH_INTERNALS_HPP__
#define __HOLYDBG_SRC_ARCH_X86_64_ARCH_INTERNALS_HPP__

#include "../arch_internals.hpp"

namespace hdbg {

class X64_ArchInternals final : public ArchInternals
{
public:
  X64_ArchInternals();
  virtual ~X64_ArchInternals();
  
  virtual std::unique_ptr<RawContext> make_raw_context() const override;
  virtual const std::vector<std::uint8_t> & make_sw_bp() const override;
};

} // namespace hdbg

#endif // __HOLYDBG_SRC_ARCH_X86_64_ARCH_INTERNALS_HPP__
