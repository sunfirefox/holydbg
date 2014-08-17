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
  Debuggee * debuggee;
  DebugThread * thread;
  process_id new_pid;
};

struct ProcessExitedEvent
{
  Debuggee * debuggee;
  int exit_code;
};

struct ProcessKilledEvent
{
  Debuggee * debuggee;
  int term_code;
};

struct ThreadCreatedEvent
{
  Debuggee * debuggee;
  DebugThread * thread;
  DebugThread * new_thread;
};

struct ThreadExitedEvent
{
  Debuggee * debuggee;
  DebugThread * thread;
  int exit_code;
};

struct UnhandledBpEvent
{
  Debuggee * debuggee;
  DebugThread * thread;
};

struct SinglestepEvent
{
  Debuggee * debuggee;
  DebugThread * thread;
};

struct AccessViolationEvent
{
  Debuggee * debuggee;
  DebugThread * thread;
};

using DebugEventListener = EventListener
<
  ProcessCreatedEvent,
  ProcessExitedEvent,
  ProcessKilledEvent,
  ThreadCreatedEvent,
  ThreadExitedEvent,
  UnhandledBpEvent,
  SinglestepEvent,
  AccessViolationEvent
>;

using DebugEventEmitter = EventEmitter
<
  ProcessCreatedEvent,
  ProcessExitedEvent,
  ProcessKilledEvent,
  ThreadCreatedEvent,
  ThreadExitedEvent,
  UnhandledBpEvent,
  SinglestepEvent,
  AccessViolationEvent
>;

} // namespace hdbg

#endif // __HDBG_DBG_DEBUG_EVENT_HPP__
