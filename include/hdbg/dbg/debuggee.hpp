#ifndef __HDBG_DEBUGGEE_HPP__
#define __HDBG_DEBUGGEE_HPP__

#include <hdbg/sys_types.hpp>
#include <hdbg/dbg/breakpoint.hpp>
#include <hdbg/dbg/debug_event.hpp>
#include <hdbg/dbg/thread_context.hpp>

#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace hdbg {

class Debuggee;
class DebugProcess;
class DebugThread;

enum DebugFlags
{
  KillOnExit = 1 << 0,
};

class Breakpoint;
typedef unsigned int breakpoint_id;

class HDBG_API Debuggee
{
public:
  typedef std::function<void(Debuggee &, DebugThread &, ThreadContext, breakpoint_id)> BpHandlerFn;
  
  virtual ~Debuggee();
  
  virtual DebugProcess & process() = 0;
  virtual const DebugProcess & process() const = 0;
  
  virtual DebugThread & get_thread(thread_id) = 0;
  virtual const DebugThread & get_thread(thread_id) const = 0;
  
  virtual bool attached() const = 0;
  virtual void singlestep(DebugThread &) = 0;
  virtual void run() = 0;
  
  virtual void detach() = 0;
  virtual void kill() = 0;
  
  virtual void add_listener(std::shared_ptr<DebugEventListener> sp_listener) = 0;
  virtual void remove_listener(const std::shared_ptr<DebugEventListener> & sp_listener) = 0;
  virtual void discard_event() = 0;
  
  virtual breakpoint_id set_bp(Breakpoint *, BpHandlerFn) = 0;
  virtual void remove_bp(breakpoint_id) = 0;
  virtual void remove_all_bps() = 0;
  
  virtual std::unique_ptr<Debuggee> attach_child(process_id pid, unsigned int flags = 0) const = 0;
};

struct DbgExecParams
{
  enum Flags
  {
    HasCwd  = 1 << 0,
    HasArgs = 1 << 1,
    HasEnv  = 1 << 2,
  };
  
  unsigned int flags = 0;
  std::string file;
  std::string cwd;
  std::vector<std::string> args;
  std::map<std::string, std::string> env;
};

} // namespace hdbg

#endif // __HDBG_DEBUGGEE_HPP__
