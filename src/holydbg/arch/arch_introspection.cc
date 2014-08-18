#include <holydbg/arch_introspection.hpp>

#include "intel/x86_64/arch_introspection.hpp"

#include <cstring>
#include <utility>
#include <stdexcept>

namespace holydbg {

ArchIntrospection & get_arch_introspection(const char * arch)
{
  static const std::pair<const char *, ArchIntrospection *> arch_intr_table[] = {
    std::make_pair("x86_64", &x64_introspection),
  };
  
  for(const auto& intr_e : arch_intr_table) {
    if(!std::strcmp(arch, intr_e.first))
      return *intr_e.second;
  } throw std::runtime_error("unknown arch_id (introspection)");
}

} // namespace holydbg
