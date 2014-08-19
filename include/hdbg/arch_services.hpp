#ifndef __HDBG_ARCH_SERVICES_HPP__
#define __HDBG_ARCH_SERVICES_HPP__

#include <hdbg/config.h>
#include <hdbg/arch/reg_info.hpp>

namespace hdbg {

class ArchInternals;

class HDBG_EXPORT ArchServices
{
public:
  virtual ~ArchServices() = default;
  
  virtual unsigned int reg_index(const char * reg_tag) const = 0;
  virtual const RegInfo & reg_info(unsigned int reg_idx) const = 0;
  virtual const std::vector<RegCategoryInfo> & reg_categories() const = 0;
  
  // virtual DecodeInstRange decode(const void * mem, std::size_t size) const = 0;
  
  virtual ArchInternals & get_internals() const = 0;
};

HDBG_EXPORT ArchServices & get_arch_services(const char * arch);

} // namespace hdbg

#endif // __HDBG_ARCH_SERVICES_HPP__
