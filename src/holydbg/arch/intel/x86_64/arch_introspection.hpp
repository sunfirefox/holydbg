#ifndef __HOLYDBG_SRC_ARCH_INTEL_X86_64_ARCH_INTROSPECTION_HPP__
#define __HOLYDBG_SRC_ARCH_INTEL_X86_64_ARCH_INTROSPECTION_HPP__

#include <holydbg/arch_introspection.hpp>

namespace holydbg {

class X64_ArchIntrospection final
  : public ArchIntrospection
{
public:
  X64_ArchIntrospection();
  virtual ~X64_ArchIntrospection();
  
  virtual const RegInfo & reg_info(unsigned int reg_idx) const override;
  virtual const std::vector<RegCategoryInfo> & reg_categories() const override;
};

extern X64_ArchIntrospection x64_introspection;

} // namespace holydbg

#endif // __HOLYDBG_SRC_ARCH_INTEL_X86_64_ARCH_INTROSPECTION_HPP__
