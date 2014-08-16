#ifndef __HDBG_DBG_DEBUG_EVENT_HPP__
#define __HDBG_DBG_DEBUG_EVENT_HPP__

#include <hdbg/sys_types.hpp>
#include <hdbg/utils/event_emitter.hpp>
#include <hdbg/utils/event_listener.hpp>

namespace hdbg {

class Debuggee;
class DebugThread;

struct ProcessCreatedEvent
{
  Debuggee * instance;
  DebugThread * thr;
  process_id new_pid;
};

struct ProcessExitedEvent
{
  Debuggee * instance;
  int exit_code;
};

struct ProcessKilledEvent
{
  Debuggee * instance;
  int term_code;
};

struct ThreadCreatedEvent
{
  Debuggee * instance;
  DebugThread * thr;
  DebugThread * new_thr;
};

struct ThreadExitedEvent
{
  Debuggee * instance;
  DebugThread * thr;
  int exit_code;
};

struct AccessViolationEvent
{
  Debuggee * instance;
  DebugThread * thr;
};

using DebugEventListener = EventListener
<
  ProcessCreatedEvent,
  ProcessExitedEvent,
  ProcessKilledEvent,
  ThreadCreatedEvent,
  ThreadExitedEvent,
  AccessViolationEvent
>;

using DebugEventEmitter = EventEmitter
<
  ProcessCreatedEvent,
  ProcessExitedEvent,
  ProcessKilledEvent,
  ThreadCreatedEvent,
  ThreadExitedEvent,
  AccessViolationEvent
>;

} // namespace hdbg

#endif // __HDBG_DBG_DEBUG_EVENT_HPP__
