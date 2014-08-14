#ifndef __HOLYDBG_DEBUGGEE_HPP__
#define __HOLYDBG_DEBUGGEE_HPP__

#include <holydbg/sys_types.hpp>
#include <holydbg/dbg/breakpoint.hpp>
#include <holydbg/dbg/debug_event.hpp>

#include <cstddef>
#include <functional>
#include <memory>

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

class HDBG_EXPORT Debuggee
{
public:
  typedef std::function<void(Debuggee &, DebugThread &, ThreadContext &, breakpoint_id)> BpHandlerFn;
  
  Debuggee();
  Debuggee(const Debuggee&) = delete;
  Debuggee(Debuggee &&);
  virtual ~Debuggee();
  
  Debuggee & operator=(const Debuggee &) = delete;
  Debuggee & operator=(Debuggee &&);
  
  virtual DebugProcess & process() = 0;
  virtual const DebugProcess & process() const = 0;
  
  virtual DebugThread & get_thread(thread_id) = 0;
  virtual const DebugThread & get_thread(thread_id) const = 0;
  
  virtual void singlestep(DebugThread &) = 0;
  virtual void run() = 0;
  
  virtual void detach() = 0;
  virtual void kill() = 0;
  
  virtual breakpoint_id set_bp(Breakpoint *, BpHandlerFn) = 0;
  virtual void remove_bp(breakpoint_id) = 0;
  virtual void remove_all_bps() = 0;
  
  virtual void add_listener(std::shared_ptr<DebugEventListener> sp_listener) = 0;
  virtual void remove_listener(const std::shared_ptr<DebugEventListener> & sp_listener) = 0;
};

} // namespace hdbg

#endif // __HOLYDBG_DEBUGGEE_HPP__
