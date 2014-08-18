#ifndef __HOLYDBG_ARCH_INTROSPECTION_HPP__
#define __HOLYDBG_ARCH_INTROSPECTION_HPP__

#include <vector>

namespace holydbg {

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

class ArchIntrospection
{
public:
  virtual ~ArchIntrospection() = default;
  
  virtual const RegInfo & reg_info(unsigned int reg_idx) const = 0;
  virtual const std::vector<RegCategoryInfo> & reg_categories() const = 0;
};

ArchIntrospection & get_arch_introspection(const char * arch);

} // namespace holydbg

#endif // __HOLYDBG_ARCH_INTROSPECTION_HPP__
