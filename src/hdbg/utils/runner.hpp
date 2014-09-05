#ifndef __HDBG_SRC_UTILS_RUNNER_HPP__
#define __HDBG_SRC_UTILS_RUNNER_HPP__

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

namespace hdbg {

class Runner
{
  template <typename Fn, typename T>
  using enable_if_ret_value_is_t = typename std::enable_if <
    std::is_same< typename std::result_of<Fn>::type, T >::value, T
  >::type;
  
  template <typename Fn, typename T>
  using enable_unless_ret_value_is_t = typename std::enable_if <
    !std::is_same< typename std::result_of<Fn>::type, T >::value,
    typename std::result_of<Fn>::type
  >::type;
  
public:
  Runner();
  Runner(const Runner &) = delete;
  Runner(Runner &&);
  ~Runner();
  
  Runner & operator=(const Runner &) = delete;
  Runner & operator=(Runner &&);
  
  template <typename Fn, typename... Args>
    enable_if_ret_value_is_t<Fn(Args...), void>
      run(Fn && fn, Args&&... args);
  
  template <typename Fn, typename... Args>
    enable_unless_ret_value_is_t<Fn(Args...), void>
      run(Fn && fn, Args&&... args);
  
private:
  void do_run(std::function<void()> fn);
  
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

template <typename Fn, typename... Args>
  Runner::enable_if_ret_value_is_t<Fn(Args...), void>
    Runner::run(Fn && fn, Args&&... args)
{
  do_run([&] {
    fn(std::forward<Args>(args)...);
  });
}

template <typename Fn, typename... Args>
  Runner::enable_unless_ret_value_is_t<Fn(Args...), void>
    Runner::run(Fn && fn, Args&&... args)
{
  using ReturnT = typename std::result_of<Fn(Args...)>::type;
  
  ReturnT ret;
  do_run([&] {
    ret = fn(std::forward<Args>(args)...);
  });
  return ret;
}

} // namespace hdbg

#endif // __HDBG_SRC_UTILS_RUNNER_HPP__
