#ifndef __HOLYDBG_DBG_BP_SW_BREAKPOINT_HPP__
#define __HOLYDBG_DBG_BP_SW_BREAKPOINT_HPP__

#include <holydbg/dbg/breakpoint.hpp>

#include <cstdint>
#include <vector>

namespace hdbg {

class HDBG_EXPORT SwBreakpoint final :
  public Breakpoint
{
public:
  SwBreakpoint(std::uintptr_t addr);
  virtual ~SwBreakpoint();
  
  virtual void setup(Debuggee & debuggee) override;
  virtual void cleanup(Debuggee & debuggee) override;
  virtual bool match(const DebugThread & dbg_thr, const ThreadContext & thr_ctx) const override;
  virtual void rewind(DebugThread & dbg_thr, ThreadContext & thr_ctx) const override;
  
private:
  std::uintptr_t addr_;
  std::vector<std::uint8_t> ov_data_;
};

} // namespace hdbg

#endif // __HOLYDBG_DBG_BP_SW_BREAKPOINT_HPP__
