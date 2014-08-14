#include <holydbg/arch/raw_context.hpp>

namespace hdbg {

std::pair<void *, std::size_t> RawContext::reg_addr(unsigned int reg_idx)
{
  const auto raddr_p = const_cast<const RawContext *>(this)->reg_addr(reg_idx);
  return std::make_pair(const_cast<void *>(raddr_p.first), raddr_p.second);
}

} // namespace hdbg
