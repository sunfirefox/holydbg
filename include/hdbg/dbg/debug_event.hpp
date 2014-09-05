#ifndef __HDBG_DBG_DEBUG_EVENT_HPP__
#define __HDBG_DBG_DEBUG_EVENT_HPP__

#include <hdbg/hdbg.hpp>
#include <hdbg/sys_types.hpp>
#include <hdbg/utils/type_list.hpp>
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
  int term_sig;
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

struct FloatingPointExceptionEvent
{
  Debuggee * debuggee;
  DebugThread * thread;
};

struct IllegalInstructionEvent
{
  Debuggee * debuggee;
  DebugThread * thread;
};

namespace detail {

template <class... Events>
struct define_event_unit
{
  typedef EventEmitter<Events...> emitter;
  typedef EventListener<Events...> listener;
  typedef boost::variant<Events...> variant;
  
  template <typename Event>
  struct event_index
  {
    static_assert(type_in<Event, Events...>(), "invalid event type");
    enum { value = type_index<Event, Events...>() };
  };
};

using DebugEventUnit = define_event_unit
<
  UnknownEvent,
  ProcessCreatedEvent,
  ProcessExitedEvent,
  ProcessKilledEvent,
  ThreadCreatedEvent,
  ThreadExitedEvent,
  BreakpointHitEvent,
  SinglestepEvent,
  AccessViolationEvent,
  FloatingPointExceptionEvent,
  IllegalInstructionEvent
>;

} // namespace detail

typedef detail::DebugEventUnit::variant  DebugEvent;
typedef detail::DebugEventUnit::emitter  DebugEventEmitter;
typedef detail::DebugEventUnit::listener DebugEventListener;

template <typename T>
inline constexpr unsigned int debug_event_index()
{
  return detail::DebugEventUnit::event_index<T>::value;
}

} // namespace hdbg

#endif // __HDBG_DBG_DEBUG_EVENT_HPP__
