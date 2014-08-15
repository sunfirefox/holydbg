#ifndef __HOLYDBG_DBG_BPS_HW_BREAKPOINT_HPP__
#define __HOLYDBG_DBG_BPS_HW_BREAKPOINT_HPP__

#include <holydbg/dbg/breakpoint.hpp>

#include <cstdint>

namespace hdbg {

class HDBG_EXPORT HwBreakpoint final
  : public Breakpoint
{
public:
  HwBreakpoint(std::uintptr_t addr);
  virtual ~HwBreakpoint();
  
  virtual void setup(Debuggee & debuggee) override;
  virtual void cleanup(Debuggee & debuggee) override;
  virtual bool match(const DebugThread & dbg_thr, const ThreadContext & thr_ctx) const override;
  virtual void rewind(DebugThread & dbg_thr, ThreadContext & thr_ctx) const override;
  
private:
  std::uintptr_t addr_;
};

} // namespace hdbg

#endif // __HOLYDBG_DBG_BPS_HW_BREAKPOINT_HPP__
