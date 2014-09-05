#ifndef __HDBG_ARCH_REG_INFO_HPP__
#define __HDBG_ARCH_REG_INFO_HPP__

#include <vector>

namespace hdbg {

struct RegInfo
{
  const char * name;
  unsigned int bits;
  unsigned int rshift;
  std::vector<const RegInfo *> subregs;
};

} // namespace hdbg

#endif // __HDBG_ARCH_REG_INFO_HPP__
