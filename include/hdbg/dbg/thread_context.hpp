#ifndef __HDBG_DBG_THREAD_CONTEXT_HPP__
#define __HDBG_DBG_THREAD_CONTEXT_HPP__

#include <hdbg/config.h>
#include <hdbg/arch/endian.hpp>
#include <hdbg/arch/reg_value.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <vector>

namespace hdbg {

class RawContext;
class LocalDebugThread;

class HDBG_EXPORT ThreadContext
{
public:
  ThreadContext();
  ThreadContext(const ThreadContext &);
  ThreadContext(ThreadContext &&);
  ~ThreadContext();
  
  RegValue reg_value(unsigned int reg_idx) const;
  void set_reg(unsigned int reg_idx, const RegValue & value);
  
private:
  friend class LocalDebugThread;
  void obtain_from(const LocalDebugThread & dbg_thr);
  void apply_to(LocalDebugThread & dbg_thr) const;
  
  std::shared_ptr<RawContext> raw_ctx_;
};

} // namespace hdbg

#endif // __HDBG_DBG_THREAD_CONTEXT_HPP__
