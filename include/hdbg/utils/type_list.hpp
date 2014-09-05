#ifndef __HDBG_UTILS_TYPE_LIST_HPP__
#define __HDBG_UTILS_TYPE_LIST_HPP__

#include <type_traits>

namespace hdbg {

template <typename Type>
inline constexpr bool type_in()
{
  return false;
}

template <typename Type, typename T, typename... Ts>
inline constexpr bool type_in()
{
  return std::is_same<Type, T>::value || type_in<Type, Ts...>();
}

namespace detail {

template <typename Type>
inline constexpr int type_index_iter(unsigned int )
{
  return -1;
}

template <typename Type, typename T, typename... Ts>
inline constexpr int type_index_iter(unsigned int n)
{
  return std::is_same<Type, T>::value ? n : type_index_iter<Type,Ts...>(n + 1);
}

} // namespace detail

template <typename Type, typename... Ts>
inline constexpr int type_index()
{
  return detail::type_index_iter<Type, Ts...>(0);
}

} // namespace hdbg

#endif // __HDBG_UTILS_TYPE_LIST_HPP__
