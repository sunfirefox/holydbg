#ifndef __HDBG_ARCH_RAW_CONTEXT_HPP__
#define __HDBG_ARCH_RAW_CONTEXT_HPP__

#include <hdbg/config.h>

#include <cstddef>
#include <memory>
#include <utility>

namespace hdbg {

class LocalDebugThread;

class HDBG_EXPORT RawContext
{
public:
  virtual ~RawContext() = default;
  
  virtual bool valid_for(const char * arch) const = 0;
  virtual std::shared_ptr<RawContext> shared_clone() const = 0;
  virtual void obtain_from(const LocalDebugThread &) = 0;
  virtual void apply_to(LocalDebugThread &) const = 0;
  
  virtual std::pair<const void *, std::size_t> reg_addr(unsigned int reg_idx) const = 0;
  std::pair<void *, std::size_t> reg_addr(unsigned int reg_idx);
};

} // namespace hdbg

#endif // __HDBG_SRC_ARCH_RAW_CONTEXT_HPP__
