#ifndef __HDBG_DBG_LOCAL_DEBUG_THREAD_HPP__
#define __HDBG_DBG_LOCAL_DEBUG_THREAD_HPP__

#include <hdbg/sys_types.hpp>
#include <hdbg/dbg/debug_thread.hpp>

#include <memory>

namespace hdbg {

class LocalDebugProcess;
class ThreadContext;

class HDBG_EXPORT LocalDebugThread final : public DebugThread
{
public:
  LocalDebugThread(LocalDebugProcess & process, thread_id tid, int flags);
  LocalDebugThread(const LocalDebugThread &) = delete;
  LocalDebugThread(LocalDebugThread &&);
  virtual ~LocalDebugThread();
  
  LocalDebugThread & operator=(const LocalDebugThread &) = delete;
  LocalDebugThread & operator=(LocalDebugThread &&);
  
  virtual DebugProcess & process() override;
  virtual const DebugProcess & process() const override;
  
  virtual thread_id id() const override;
  
  virtual void get_context(ThreadContext &) const override;
  virtual void set_context(const ThreadContext &) override;
  
private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

} // namespace hdbg

#endif // __HDBG_DBG_LOCAL_DEBUG_THREAD_HPP__
