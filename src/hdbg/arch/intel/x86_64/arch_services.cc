#include "arch_services.hpp"

#include <hdbg/arch/x86_64/reg_ids.hpp>

#include "arch_internals.hpp"

#include <cstring>
#include <utility>
#include <stdexcept>

namespace hdbg {

X64_ArchServices x64_services;

X64_ArchServices::X64_ArchServices() = default;
X64_ArchServices::~X64_ArchServices() = default;

unsigned int X64_ArchServices::reg_index(const char * reg_tag) const
{
  static const std::pair<const char *, unsigned int> reg_idx_table[] = {
    std::make_pair("inst-ptr",  X64_RegRip),
    std::make_pair("stack-ptr", X64_RegRsp),
    std::make_pair("frame-ptr", X64_RegRbp),
  };
  
  for(const auto& reg_idx_e : reg_idx_table) {
    if(!std::strcmp(reg_tag, reg_idx_e.first))
      return reg_idx_e.second;
  } throw std::invalid_argument("unknown reg tag");
}

ArchInternals & X64_ArchServices::get_internals() const
{
  static X64_ArchInternals x64_internals;
  return x64_internals;
}

} // namespace hdbg
