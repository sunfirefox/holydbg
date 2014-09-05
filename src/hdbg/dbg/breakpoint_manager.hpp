#ifndef __HDBG_SRC_DBG_BREAKPOINT_MANAGER_HPP__
#define __HDBG_SRC_DBG_BREAKPOINT_MANAGER_HPP__

#include <hdbg/dbg/breakpoint.hpp>
#include <hdbg/dbg/debuggee.hpp>
#include <hdbg/dbg/debug_event.hpp>
#include <hdbg/dbg/thread_context.hpp>

#include <map>
#include <memory>

namespace hdbg {

class Debuggee;
class DebugThread;

class BreakpointManager
{
public:
  typedef Debuggee::BpHandlerFn BpHandlerFn;
  
  BreakpointManager();
  BreakpointManager(const BreakpointManager &) = delete;
  BreakpointManager(BreakpointManager &&);
  ~BreakpointManager();
  
  breakpoint_id set_bp(Debuggee & debuggee, Breakpoint * bp, BpHandlerFn handler);
  void remove_bp(breakpoint_id bp_id);
  void remove_all_bps(Debuggee * debuggee = nullptr);
  
  bool dispatch_bp_hit(Debuggee & debuggee, DebugThread & dbg_thr,
                       const DebugEvent & dbg_evt);
  void restore_bps();
  
private:
  
  struct BreakpointData
  {
    std::unique_ptr<Breakpoint> bp_ptr;
    Debuggee * debuggee;
    BpHandlerFn handler;
    bool active;
  };
  
  ThreadContext thr_ctx_;
  breakpoint_id next_id_;
  std::map<breakpoint_id, BreakpointData> bps_;
};

} // namespace hdbg

#endif // __HDBG_SRC_DBG_BREAKPOINT_MANAGER_HPP__
