#include "breakpoint_manager.hpp"

#include <hdbg/dbg/debug_thread.hpp>
#include <hdbg/dbg/thread_context.hpp>

namespace hdbg {

BreakpointManager::BreakpointManager() : next_id_(1) {}

BreakpointManager::BreakpointManager(BreakpointManager &&) = default;
BreakpointManager::~BreakpointManager() = default;

breakpoint_id BreakpointManager::set_bp(Debuggee & debuggee, Breakpoint * bp,
                                        BpHandlerFn handler)
{
  if(!bp)
    throw std::invalid_argument("null bp");
  
  std::unique_ptr<Breakpoint> bp_ptr ( bp );
  bp_ptr->setup(debuggee);
  bps_.emplace(next_id_, BreakpointData{ std::move(bp_ptr), &debuggee, handler, true });
  return next_id_++;
}

void BreakpointManager::remove_bp(breakpoint_id bp_id)
{
  auto& bp_data = bps_.at(bp_id);
  if(bp_data.active)
    bp_data.bp_ptr->cleanup(*bp_data.debuggee);
  bps_.erase(bp_id);
}

void BreakpointManager::remove_all_bps(Debuggee * debuggee)
{
  for(const auto& bp_e : bps_) {
    if(debuggee == nullptr || bp_e.second.debuggee == debuggee) {
      if(bp_e.second.active)
        bp_e.second.bp_ptr->cleanup(*bp_e.second.debuggee);
      bps_.erase(bp_e.first);
    }
  }
}

bool BreakpointManager::dispatch_bp_hit(Debuggee & debuggee, DebugThread & dbg_thr,
                                        const DebugEvent & dbg_evt)
{
  dbg_thr.get_context(thr_ctx_);
  
  bool bp_hit = false;
  for(auto& bp_e : bps_) {
    if(bp_e.second.active && bp_e.second.debuggee == &debuggee &&
       bp_e.second.bp_ptr->match(dbg_thr, dbg_evt, thr_ctx_))
    {
      const breakpoint_id bp_id = bp_e.first;
      auto& bp_ptr = bp_e.second.bp_ptr;
      
      bp_ptr->rewind(dbg_thr, thr_ctx_);
      bp_e.second.handler(debuggee, dbg_thr, thr_ctx_, bp_id);
      bp_hit = true;
      
      bp_ptr->cleanup(debuggee);
      bp_e.second.active = false;
      break;
    }
  }
  return bp_hit;
}

void BreakpointManager::restore_bps()
{
  for(auto& bp_e : bps_) {
    if(!bp_e.second.active) {
      auto& bp_ptr = bp_e.second.bp_ptr;
      bp_ptr->setup(*bp_e.second.debuggee);
      bp_e.second.active = true;
    }
  }
}

} // namespace hdbg
