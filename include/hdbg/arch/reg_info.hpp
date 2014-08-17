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

struct RegCategoryInfo
{
  const char * name;
  std::vector<unsigned int> reg_indexes;
};

} // namespace hdbg

#endif // __HDBG_ARCH_REG_INFO_HPP__
