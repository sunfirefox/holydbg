#ifndef __HDBG_DBG_BPS_HW_BREAKPOINT_HPP__
#define __HDBG_DBG_BPS_HW_BREAKPOINT_HPP__

#include <hdbg/sys_types.hpp>
#include <hdbg/dbg/breakpoint.hpp>
#include <hdbg/dbg/debug_event.hpp>
#include <hdbg/dbg/thread_context.hpp>

#include <cstdint>
#include <memory>
#include <unordered_map>

namespace hdbg {

class DebugThread;

class HDBG_API HwBreakpoint final
  : public Breakpoint
{
  class HwBpxDbgEvtListener final
    : public DebugEventListener
  {
  public:
    HwBpxDbgEvtListener(HwBreakpoint &);
    
  private:
    virtual void handle_event(const ThreadCreatedEvent &) override;
    virtual void handle_event(const ThreadExitedEvent &) override;
    
    HwBreakpoint & hw_bpx_;
  };
  
public:
  HwBreakpoint(std::uintptr_t addr);
  virtual ~HwBreakpoint();
  
  virtual void setup(Debuggee & debuggee) override;
  virtual void cleanup(Debuggee & debuggee) override;
  virtual bool match(const DebugThread & dbg_thr, const DebugEvent & dbg_evt,
                     const ThreadContext & thr_ctx) const override;
  virtual void rewind(DebugThread & dbg_thr, ThreadContext & thr_ctx) const override;
  
private:
  void set_on_thread(DebugThread &);
  void remove_from_thread(DebugThread &);
  
  struct HwBpxData
  {
    unsigned int dreg_idx;
    RegValue ov_dreg_val;
  };
  
  std::uintptr_t addr_;
  std::unordered_map<thread_id, HwBpxData> bp_regs_;
  std::shared_ptr<HwBpxDbgEvtListener> evt_listener_;
  ThreadContext thr_ctx_;
};

} // namespace hdbg

#endif // __HDBG_DBG_BPS_HW_BREAKPOINT_HPP__
