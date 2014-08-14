#ifndef __HOLYDBG_DBG_BREAKPOINT_HPP__
#define __HOLYDBG_DBG_BREAKPOINT_HPP__

#include <holydbg/config.h>

namespace hdbg {

class Debuggee;
class DebugThread;
class ThreadContext;

class HDBG_EXPORT Breakpoint
{
public:
  virtual ~Breakpoint() = default;
  virtual void setup(Debuggee & debuggee) = 0;
  virtual void cleanup(Debuggee & debuggee) = 0;
  virtual bool match(const DebugThread & dbg_thr, const ThreadContext & thr_ctx) const = 0;
  virtual void rewind(DebugThread & debuggee, ThreadContext & thr_ctx) const = 0;
};

} // namespace hdbg

#endif // __HOLYDBG_DBG_BREAKPOINT_HPP__
