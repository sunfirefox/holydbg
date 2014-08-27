#ifndef __HDBG_DBG_DEBUG_THREAD_HPP__
#define __HDBG_DBG_DEBUG_THREAD_HPP__

#include <hdbg/config.h>
#include <hdbg/sys_types.hpp>

#include <memory>

namespace hdbg {

class DebugProcess;
class ThreadContext;

class HDBG_API DebugThread
{
public:
  
  enum OpenFlags
  {
    ContextGet = 1 << 0,
    ContextSet = 1 << 1,
    
    AllAccess = ContextGet | ContextSet
  };
  
  virtual ~DebugThread() = default;
  
  virtual DebugProcess & process() = 0;
  virtual const DebugProcess & process() const = 0;
  
  virtual thread_id id() const = 0;
  
  virtual void get_context(ThreadContext &) const = 0;
  virtual void set_context(const ThreadContext &) = 0;
};

} // namespace hdbg

#endif // __HDBG_DBG_DEBUG_THREAD_HPP__
