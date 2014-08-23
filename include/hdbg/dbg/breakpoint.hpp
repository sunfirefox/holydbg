#ifndef __HDBG_DBG_BREAKPOINT_HPP__
#define __HDBG_DBG_BREAKPOINT_HPP__

#include <hdbg/config.h>
#include <hdbg/dbg/debug_event.hpp>

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
  virtual bool match(const DebugThread & dbg_thr, const DebugEvent & dbg_evt,
                     const ThreadContext & thr_ctx) const = 0;
  virtual void rewind(DebugThread & debuggee, ThreadContext & thr_ctx) const = 0;
};

} // namespace hdbg

#endif // __HDBG_DBG_BREAKPOINT_HPP__
