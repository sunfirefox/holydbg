#ifndef __HOLYDBG_ARCH_ENDIAN_HPP__
#define __HOLYDBG_ARCH_ENDIAN_HPP__

#include <holydbg/config.h>

#include <algorithm>
#include <cstdint>
#include <cstddef>

namespace hdbg {

namespace detail {

template <std::size_t N> inline
void ecopy_N(const void * from, void * to)
{
  const auto bytes_from = reinterpret_cast<const std::uint8_t *>(from);
  const auto bytes_to = reinterpret_cast<std::uint8_t *>(to);
  std::copy(bytes_from, bytes_from + N, bytes_to);
}

template <std::size_t N> inline
void ecopy_rev_N(const void * from, void * to)
{
  const auto bytes_from = reinterpret_cast<const std::uint8_t *>(from);
  const auto bytes_to = reinterpret_cast<std::uint8_t *>(to);
  ecopy_rev_N<N / 2>(bytes_from, bytes_to + N / 2);
  ecopy_rev_N<N / 2>(bytes_from + N / 2, bytes_to);
}

template <> inline
void ecopy_rev_N<sizeof(std::uint8_t)>(const void * from, void * to)
{
  const auto bytes_from = reinterpret_cast<const std::uint8_t *>(from);
  const auto bytes_to = reinterpret_cast<std::uint8_t *>(to);
  *bytes_to = *bytes_from;
}

#ifdef HOLYDBG_ENDIAN_BE

template <std::size_t N> inline
void be_read_N(const void * from, void * to)
{
  ecopy_N<N>(from, to);
}

template <std::size_t size> inline
void le_read_N(const void * from, void * to)
{
  ecopy_rev_N<N>(from, to);
}

template <std::size_t N> inline
void be_write_N(void * to, const void * from)
{
  ecopy_N<N>(from, to);
}

template <std::size_t size> inline
void le_write_N(void * to, const void * from)
{
  ecopy_rev_N(from, to);
}

#else // HOLYDBG_ENDIAN_BE

template <std::size_t N> inline
void be_read_N(const void * from, void * to)
{
  ecopy_rev_N<N>(from, to);
}

template <std::size_t N> inline
void le_read_N(const void * from, void * to)
{
  ecopy_N<N>(from, to);
}

template <std::size_t N> inline
void be_write_N(void * to, const void * from)
{
  ecopy_rev_N<N>(from, to);
}

template <std::size_t N> inline
void le_write_N(void * to, const void * from)
{
  ecopy_N<N>(from, to);
}

#endif // HOLYDBG_ENDIAN_BE

} // namespace detail

template <typename T> inline
T be_load(const void * from)
{
  T into;
  detail::be_read_N<sizeof(T)>(from, &into);
  return into;
}

template <typename T> inline
T le_load(const void * from)
{
  T into;
  detail::le_read_N<sizeof(T)>(from, &into);
  return into;
}

template <typename T> inline
void be_load_into(const void * from, T & out)
{
  out = be_load<T>(from);
}

template <typename T, std::size_t N>
void be_load_into(const void * from, T (&out)[N])
{
  const auto tsfrom = reinterpret_cast<const T *>(from);
  for(unsigned int i = 0; i < N; ++i)
    out[i] = be_load<T>(tsfrom + i);
}

template <typename T> inline
void le_load_into(const void * from, T & out)
{
  out = le_load<T>(from);
}

template <typename T, std::size_t N>
void le_load_into(const void * from, T (&out)[N])
{
  const auto tsfrom = reinterpret_cast<const T *>(from);
  for(unsigned int i = 0; i < N; ++i)
    out[i] = le_load<T>(tsfrom + i);
}

template <typename T> inline
void be_store(void * where, const T & what)
{
  detail::be_write_N<sizeof(T)>(where, &what);
}

template <typename T, std::size_t N>
void be_store(void * where, const T (&what)[N])
{
  const auto tswhere = reinterpret_cast<T *>(where);
  for(unsigned int i = 0; i < N; ++i)
    be_store(tswhere + i, what[i]);
}

template <typename T> inline
void le_store(void * where, const T & what)
{
  detail::le_write_N<sizeof(T)>(where, &what);
}

template <typename T, std::size_t N>
void le_store(void * where, const T (&what)[N])
{
  const auto tswhere = reinterpret_cast<T *>(where);
  for(unsigned int i = 0; i < N; ++i)
    le_store(tswhere + i, what[i]);
}

template <typename T> inline
T native_load(const void * where)
{
#ifdef HOLYDBG_ENDIAN_BE
  return be_load<T>(where);
#else  // HOLYDBG_ENDIAN_BE
  return le_load<T>(where);
#endif // HOLYDBG_ENDIAN_BE
}

template <typename T> inline
void native_load_into(const void * where, T & out)
{
#ifdef HOLYDBG_ENDIAN_BE
  be_load_into(where, out);
#else  // HOLYDBG_ENDIAN_BE
  le_load_into(where, out);
#endif // HOLYDBG_ENDIAN_BE
}

template <typename T> inline
void native_store(void * where, const T & what)
{
#ifdef HOLYDBG_ENDIAN_BE
  be_store(where, what);
#else  // HOLYDBG_ENDIAN_BE
  le_store(where, what);
#endif // HOLYDBG_ENDIAN_BE
}

} // namespace hdbg

#endif // __HOLYDBG_ARCH_ENDIAN_HPP__
