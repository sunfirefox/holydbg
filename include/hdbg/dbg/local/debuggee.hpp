#ifndef __HDBG_DBG_LOCAL_DEBUGGEE_HPP__
#define __HDBG_DBG_LOCAL_DEBUGGEE_HPP__

#include <hdbg/sys_types.hpp>
#include <hdbg/dbg/debuggee.hpp>
#include <hdbg/dbg/debug_event.hpp>
#include <hdbg/dbg/breakpoint_manager.hpp>
#include <hdbg/dbg/local/debug_process.hpp>
#include <hdbg/dbg/local/debug_thread.hpp>

#include <memory>

namespace hdbg {

class HDBG_EXPORT LocalDebuggee final
  : public Debuggee
{
public:
  static std::unique_ptr<LocalDebuggee> exec(const DbgExecParams & params, unsigned int flags = 0);
  static std::unique_ptr<LocalDebuggee> attach(process_id pid, unsigned int flags = 0);
  
  LocalDebuggee(const LocalDebuggee &) = delete;
  LocalDebuggee(LocalDebuggee &&);
  virtual ~LocalDebuggee();
  
  virtual LocalDebugProcess & process() override;
  virtual const LocalDebugProcess & process() const override;
  
  virtual LocalDebugThread & get_thread(thread_id) override;
  virtual const LocalDebugThread & get_thread(thread_id) const override;
  
  virtual bool attached() const override;
  
  virtual void singlestep(DebugThread &) override;
  virtual void run_until_next_event() override;
  
  virtual void detach() override;
  virtual void kill() override;
  
  virtual void add_listener(std::shared_ptr<DebugEventListener> sp_listener) override;
  virtual void remove_listener(const std::shared_ptr<DebugEventListener> & sp_listener) override;
  virtual void discard_event() override;
  
  virtual breakpoint_id set_bp(Breakpoint * bp_ptr, BpHandlerFn bp_fn) override;
  virtual void remove_bp(breakpoint_id bp_id) override;
  virtual void remove_all_bps() override;
  
  virtual std::unique_ptr<Debuggee> attach_child(process_id pid, unsigned int flags = 0) const override;
  
private:
  LocalDebuggee(process_id pid);
  LocalDebuggee(const LocalDebuggee & parent, process_id pid);
  
  DebugEventEmitter evt_emitter_;
  BreakpointManager bp_mgr_;
  
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

// HDBG_EXPORT std::unique_ptr<LocalDebuggee> dbg_exec(const DbgExecParams & params, unsigned int flags = 0);
// HDBG_EXPORT std::unique_ptr<LocalDebuggee> dbg_attach(process_id pid, unsigned int flags = 0);

} // namespace hdbg

#endif // __HDBG_DBG_LOCAL_DEBUGGEE_HPP__
