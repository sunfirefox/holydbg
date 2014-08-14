#include "arch_services.hpp"

#include "arch_internals.hpp"
#include "reg_ids.hpp"

#include <cstring>
#include <utility>

namespace hdbg {

X64_ArchServices x64_services;

X64_ArchServices::X64_ArchServices() = default;
X64_ArchServices::~X64_ArchServices() = default;

int X64_ArchServices::reg_index(const char * reg_desc) const
{
  static const std::pair<const char *, unsigned int> reg_idx_table[] = {
    std::make_pair("inst-ptr",  X64_RegRip),
    std::make_pair("stack-ptr", X64_RegRsp),
    std::make_pair("frame-ptr", X64_RegRbp),
  };
  
  for(const auto& reg_idx_e : reg_idx_table) {
    if(!std::strcmp(reg_desc, reg_idx_e.first))
      return reg_idx_e.second;
  }
  return -1;
}

ArchInternals & X64_ArchServices::get_internals() const
{
  static X64_ArchInternals x64_internals;
  return x64_internals;
}

} // namespace hdbg
