#include <hdbg/arch_services.hpp>

#include "intel/x86_64/arch_services.hpp"

#include <cstring>
#include <utility>
#include <stdexcept>

namespace hdbg {

ArchServices & get_arch_services(const char * arch)
{
  static const std::pair<const char *, ArchServices *> arch_svc_table[] = {
    std::make_pair("x86_64", &x64_services),
  };
  
  for(const auto& svc_e : arch_svc_table) {
    if(!std::strcmp(arch, svc_e.first))
      return *svc_e.second;
  } throw std::runtime_error("unknown arch_id (services)");
}

} // namespace hdbg
