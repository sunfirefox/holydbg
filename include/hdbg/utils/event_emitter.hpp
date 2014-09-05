#ifndef __HDBG_UTILS_EVENT_EMITTER_HPP__
#define __HDBG_UTILS_EVENT_EMITTER_HPP__

#include <hdbg/utils/type_list.hpp>
#include <hdbg/utils/event_listener.hpp>

#include <algorithm>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace hdbg {

template <class... Events>
class EventEmitter
{
public:
  EventEmitter() = default;
  EventEmitter(const EventEmitter & emitter) = default;
  EventEmitter(EventEmitter && emitter) = default;
  ~EventEmitter() = default;
  
  EventEmitter & operator=(const EventEmitter &) = default;
  EventEmitter & operator=(EventEmitter &&) = default;
  
  void add_listener(std::shared_ptr<EventListener<Events...>> sp_listener);
  void remove_listener(const std::shared_ptr<EventListener<Events...>> & sp_listener);
  
  template <typename Event>
  void emit_event(const Event & event) const;
  
private:
  mutable std::vector<std::weak_ptr<EventListener<Events...>>> listeners_;
};

template <class... Events>
void EventEmitter<Events...>::add_listener(std::shared_ptr<EventListener<Events...>> sp_listener)
{
  listeners_.emplace_back(std::move(sp_listener));
}

template <class... Events>
void EventEmitter<Events...>::remove_listener(const std::shared_ptr<EventListener<Events...>> & sp_listener)
{
  listeners_.erase(std::remove_if(listeners_.begin(), listeners_.end(),
    [&sp_listener] (std::weak_ptr<EventListener<Events...>>& wp_listener){
      const auto listener_ptr = wp_listener.lock();
      return !listener_ptr || listener_ptr == sp_listener;
    }), listeners_.end());
}

template <class... Events> template <class Event>
void EventEmitter<Events...>::emit_event(const Event & event) const
{
  static_assert( type_in<Event, Events...>(), "invalid event type");
  
  listeners_.erase(std::remove_if(listeners_.begin(), listeners_.end(),
    [&event](std::weak_ptr<EventListener<Events...>>& wp_listener)
    {
      const auto listener_ptr = wp_listener.lock();
      if(!listener_ptr)
        return true;
      
      listener_ptr->on(event);
      return false;
    }), listeners_.end());
}

} // namespace hdbg

#endif // __HDBG_UTILS_EVENT_EMITTER_HPP__
