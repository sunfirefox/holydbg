#ifndef __HDBG_UTILS_EVENT_LISTENER_HPP__
#define __HDBG_UTILS_EVENT_LISTENER_HPP__

#include <hdbg/hdbg.hpp>

namespace hdbg {

template <class... Events>
class EventListener;

template <class Event, class... Events>
class EventListener<Event, Events...>
{
public:
  virtual ~EventListener() = default;
  virtual void on(const Event &) {}
};

template <class Event, class EventNext, class... Events>
class EventListener<Event, EventNext, Events...>
  : public EventListener<EventNext, Events...>
{
public:
  virtual ~EventListener() = default;
  using EventListener<EventNext, Events...>::on;
  virtual void on(const Event &) {}
};

} // namespace hdbg

#endif // __HDBG_UTILS_EVENT_LISTENER_HPP__
