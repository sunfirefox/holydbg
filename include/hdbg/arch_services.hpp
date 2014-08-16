#ifndef __HDBG_ARCH_SERVICES_HPP__
#define __HDBG_ARCH_SERVICES_HPP__

#include <hdbg/dbg/thread_context.hpp>

namespace hdbg {

class ArchInternals;

class HDBG_EXPORT ArchServices
{
public:
  virtual ~ArchServices() = default;
  virtual unsigned int reg_index(const char * reg_tag) const = 0;
  virtual ArchInternals & get_internals() const = 0;
};

HDBG_EXPORT ArchServices & get_arch_services(const char * arch);

} // namespace hdbg

#endif // __HDBG_ARCH_SERVICES_HPP__
