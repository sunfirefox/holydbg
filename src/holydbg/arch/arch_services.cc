#include <holydbg/arch_services.hpp>

#include "intel/x86_64/arch_services.hpp"

#include <cstring>
#include <utility>
#include <stdexcept>

namespace hdbg {

ArchServices & get_arch_services(const char * arch)
{
  static const std::pair<const char *, ArchServices *> arch_table[] = {
    std::make_pair("x86_64", &x64_services),
  };
  
  for(const auto& arch_e : arch_table) {
    if(!std::strcmp(arch, arch_e.first))
      return *arch_e.second;
  }
  
  throw std::runtime_error("unknown arch (svc)");
}

} // namespace hdbg
