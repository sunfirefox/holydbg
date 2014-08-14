#ifndef __HOLYDBG_ARCH_RAW_CONTEXT_HPP__
#define __HOLYDBG_ARCH_RAW_CONTEXT_HPP__

#include <holydbg/config.h>

#include <cstddef>
#include <utility>

namespace hdbg {

class LocalDebugThread;

class HDBG_EXPORT RawContext
{
public:
  virtual ~RawContext() = default;
  
  virtual bool valid_for(const char * arch) const = 0;
  virtual void obtain_from(const LocalDebugThread &) = 0;
  virtual void apply_to(LocalDebugThread &) const = 0;
  
  virtual std::pair<const void *, std::size_t> reg_addr(unsigned int reg_idx) const = 0;
  std::pair<void *, std::size_t> reg_addr(unsigned int reg_idx);
};

} // namespace hdbg

#endif // __HOLYDBG_SRC_ARCH_RAW_CONTEXT_HPP__
