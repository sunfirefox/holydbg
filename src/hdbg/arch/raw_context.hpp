#ifndef __HDBG_SRC_ARCH_RAW_CONTEXT_HPP__
#define __HDBG_SRC_ARCH_RAW_CONTEXT_HPP__

#include <hdbg/arch/reg_value.hpp>

#include <memory>

#include <cstddef>
#include <utility>

namespace hdbg {

class LocalDebugThread;

class HDBG_EXPORT RawContext
{
public:
  virtual ~RawContext() = default;
  
  virtual std::unique_ptr<RawContext> clone() const = 0;
  virtual std::shared_ptr<RawContext> shared_clone() const = 0;
  
  virtual bool valid_for(const LocalDebugThread &) const = 0;
  virtual void obtain_from(const LocalDebugThread &) = 0;
  virtual void apply_to(LocalDebugThread &) const = 0;
  
  virtual RegValue reg_value(unsigned int reg_idx) const = 0;
  virtual void set_reg(unsigned int reg_idx, const RegValue & value) = 0;
};

} // namespace hdbg

#endif // __HDBG_SRC_ARCH_RAW_CONTEXT_HPP__
