#ifndef __HDBG_DBG_DEBUG_EVENT_HPP__
#define __HDBG_DBG_DEBUG_EVENT_HPP__

#include <hdbg/sys_types.hpp>
#include <hdbg/utils/event_emitter.hpp>
#include <hdbg/utils/event_listener.hpp>

#include <boost/variant.hpp>

namespace hdbg {

class Debuggee;
class DebugThread;

struct UnknownEvent
{
  Debuggee * debuggee;
};

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

struct BreakpointHitEvent
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

typedef boost::variant
<
  UnknownEvent,
  ProcessCreatedEvent,
  ProcessExitedEvent,
  ProcessKilledEvent,
  ThreadCreatedEvent,
  ThreadExitedEvent,
  BreakpointHitEvent,
  SinglestepEvent,
  AccessViolationEvent
> DebugEvent;

enum {
  UnknownEventId,
  ProcessCreatedEventId,
  ProcessExitedEventId,
  ProcessKilledEventId,
  ThreadCreatedEventId,
  ThreadExitedEventId,
  BreakpointHitEventId,
  SinglestepEventId,
  AccessViolationEventId
} DebugEventId;

typedef EventListener
<
  UnknownEvent,
  ProcessCreatedEvent,
  ProcessExitedEvent,
  ProcessKilledEvent,
  ThreadCreatedEvent,
  ThreadExitedEvent,
  BreakpointHitEvent,
  SinglestepEvent,
  AccessViolationEvent
> DebugEventListener;

typedef EventEmitter
<
  UnknownEvent,
  ProcessCreatedEvent,
  ProcessExitedEvent,
  ProcessKilledEvent,
  ThreadCreatedEvent,
  ThreadExitedEvent,
  BreakpointHitEvent,
  SinglestepEvent,
  AccessViolationEvent
> DebugEventEmitter;

} // namespace hdbg

#endif // __HDBG_DBG_DEBUG_EVENT_HPP__
