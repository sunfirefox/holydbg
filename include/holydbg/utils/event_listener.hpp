#ifndef __HOLYDBG_UTILS_EVENT_LISTENER_HPP__
#define __HOLYDBG_UTILS_EVENT_LISTENER_HPP__

#include <stdexcept>
#include <type_traits>

namespace hdbg {

namespace detail {

template <typename Type>
constexpr bool type_in()
{
  return false;
}

template <typename Type, typename T, typename... Ts>
constexpr bool type_in()
{
  return std::is_same<Type, T>::value || type_in<Type, Ts...>();
}

} // namespace detail

template <class... Events>
class EventListener
{
public:
  virtual ~EventListener() = default;
  
  template <typename T>
  void on(const T & evt)
  {
    static_assert( detail::type_in<T>(), "invalid event type" );
  }
};

template <class Event, class... Events>
class EventListener<Event, Events...>
  : public EventListener<Events...>
{
public:
  virtual ~EventListener() = default;
  
  template <typename T>
  typename std::enable_if<std::is_same<T, Event>::value>::type on(const T & evt)
  {
    handle_event(evt);
  }
  
  template <typename T>
  typename std::enable_if<!std::is_same<T, Event>::value>::type on(const T& evt)
  {
    static_assert( detail::type_in<T, Event, Events...>(), "invalid event type");
    
    EventListener<Events...>::on(evt);
  }
  
private:
  virtual void handle_event(const Event &) {}
};

} // namespace hdbg

#endif // __HOLYDBG_UTILS_EVENT_LISTENER_HPP__
