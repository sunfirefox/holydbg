#ifndef __HOLYDBG_SRC_UTILS_VOLATILE_MODIFY_HPP__
#define __HOLYDBG_SRC_UTILS_VOLATILE_MODIFY_HPP__

#include <utility>

namespace hdbg {

template <typename T>
class RestoreVariableGuard
{
public:
  RestoreVariableGuard(T & var);
  RestoreVariableGuard(const RestoreVariableGuard &) = delete;
  RestoreVariableGuard(RestoreVariableGuard &&);
  ~RestoreVariableGuard();
  
  RestoreVariableGuard & operator=(const RestoreVariableGuard &) = delete;
  RestoreVariableGuard & operator=(RestoreVariableGuard &&);
  
  void cancel();
  void reset();
  
private:
  T * var_ = nullptr;
  T value_;
};

template <typename T>
RestoreVariableGuard<T>::RestoreVariableGuard(T & var)
  : var_( &var ), value_( var ) {}

template <typename T>
RestoreVariableGuard<T>::RestoreVariableGuard(RestoreVariableGuard && rv_guard)
  : var_( rv_guard.var_ )
  , value_( rv_guard.value_ )
{
  rv_guard.var_ = nullptr;
}

template <typename T>
RestoreVariableGuard<T>::~RestoreVariableGuard()
{
  if(var_)
    *var_ = value_;
}

template <typename T>
RestoreVariableGuard<T> & RestoreVariableGuard<T>::operator=(RestoreVariableGuard && rv_guard)
{
  reset();
  var_ = rv_guard.var_;
  value_ = rv_guard.value_;
  rv_guard.var_ = nullptr;
}

template <typename T>
void RestoreVariableGuard<T>::cancel()
{
  var_ = nullptr;
}

template <typename T>
void RestoreVariableGuard<T>::reset()
{
  if(var_) {
    *var_ = value_;
    var_ = nullptr;
  }
}

template <typename Variable, typename Value>
RestoreVariableGuard<Variable> volatile_modify(Variable & var, Value && value)
{
  RestoreVariableGuard<Variable> rv_guard ( var );
  var = std::forward<Value>(value);
  return rv_guard;
}

} // namespace hdbg

#endif // __HOLYDBG_SRC_UTILS_VOLATILE_MODIFY_HPP__
