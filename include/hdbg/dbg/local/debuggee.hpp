#ifndef __HDBG_DBG_LOCAL_DEBUGGEE_HPP__
#define __HDBG_DBG_LOCAL_DEBUGGEE_HPP__

#include <hdbg/sys_types.hpp>
#include <hdbg/dbg/debuggee.hpp>
#include <hdbg/dbg/debug_event.hpp>
#include <hdbg/dbg/breakpoint_manager.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace hdbg {

class LocalDebugProcess;

class HDBG_EXPORT LocalDebuggee final : public Debuggee
{
public:
  LocalDebuggee(LocalDebugProcess &&, int dbgflags);
  LocalDebuggee(const LocalDebuggee &) = delete;
  LocalDebuggee(LocalDebuggee &&);
  virtual ~LocalDebuggee();
  
  virtual DebugProcess & process() override;
  virtual const DebugProcess & process() const override;
  
  virtual DebugThread & get_thread(thread_id) override;
  virtual const DebugThread & get_thread(thread_id) const override;
  
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
  
private:
  DebugEventEmitter evt_emitter_;
  BreakpointManager bp_mgr_;
  
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

struct ExecParams
{
  enum Flags
  {
    HasArgs = 1 << 0,
    HasEnv  = 1 << 1,
  };
  
  unsigned int flags = 0;
  std::string file;
  std::vector<std::string> args;
  std::map<std::string, std::string> env;
};

HDBG_EXPORT std::unique_ptr<Debuggee> dbg_exec(const ExecParams & params, unsigned int flags = 0);
HDBG_EXPORT std::unique_ptr<Debuggee> dbg_attach(process_id pid, unsigned int flags = 0);

} // namespace hdbg

#endif // __HDBG_DBG_LOCAL_DEBUGGEE_HPP__
