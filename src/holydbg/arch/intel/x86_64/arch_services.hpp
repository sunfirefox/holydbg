#ifndef __HOLYDBG_SRC_ARCH_INTEL_X86_64_ARCH_SERVICES_HPP__
#define __HOLYDBG_SRC_ARCH_INTEL_X86_64_ARCH_SERVICES_HPP__

#include <holydbg/arch_services.hpp>

namespace hdbg {

class X64_ArchServices final : public ArchServices
{
public:
  X64_ArchServices();
  virtual ~X64_ArchServices();
  
  virtual unsigned int reg_index(const char * reg_tag) const override;
  virtual ArchInternals & get_internals() const override;
};

extern X64_ArchServices x64_services;

} // namespace hdbg

#endif // __HOLYDBG_SRC_ARCH_INTEL_X86_64_ARCH_SERVICES_HPP__
