#ifndef __HOLYDBG_ARCH_SERVICES_HPP__
#define __HOLYDBG_ARCH_SERVICES_HPP__

#include <holydbg/dbg/thread_context.hpp>

namespace hdbg {

class ArchInternals;

class HDBG_EXPORT ArchServices
{
public:
  virtual ~ArchServices() = default;
  virtual int reg_index(const char * reg_desc) const = 0;
  virtual ArchInternals & get_internals() const = 0;
};

HDBG_EXPORT ArchServices & get_arch_services(const char * arch);

} // namespace hdbg

#endif // __HOLYDBG_ARCH_SERVICES_HPP__
