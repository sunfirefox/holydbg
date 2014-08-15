#ifndef __HOLYDBG_DBG_THREAD_CONTEXT_HPP__
#define __HOLYDBG_DBG_THREAD_CONTEXT_HPP__

#include <holydbg/config.h>
#include <holydbg/arch/endian.hpp>
#include <holydbg/arch/raw_context.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <vector>

#include <iostream>

namespace hdbg {

class LocalDebugThread;

class HDBG_EXPORT ThreadContext
{
  friend class LocalDebugThread;
  
  template <typename T>
  static constexpr bool is_valid_reg_value()
  {
    return std::is_integral<T>::value &&
           std::is_unsigned<T>::value ;
  }
  
public:
  ThreadContext();
  ThreadContext(const ThreadContext &);
  ThreadContext(ThreadContext &&);
  ~ThreadContext();
  
  template <typename T> // TODO: remove
  T reg_value(unsigned int reg_idx) const;
  
  template <typename T>
  T get_reg(unsigned int reg_idx) const;
  
  template <typename T> 
  void set_reg(unsigned int reg_idx, const T & value);
  
private:
  void do_get_thread_context(const LocalDebugThread & dbg_thr);
  void do_set_thread_context(LocalDebugThread & dbg_thr) const;
  
  std::unique_ptr<RawContext> raw_ctx_;
};

template <typename T>
T ThreadContext::reg_value(unsigned int reg_idx) const
{
  static_assert( is_valid_reg_value<T>(), "invalid reg value type" );
  
  if(!raw_ctx_)
    throw std::out_of_range("invalid reg index");
  
  const auto raddr_p = raw_ctx_->reg_addr(reg_idx);
  std::vector<std::uint8_t> buf( std::max(raddr_p.second, sizeof(T)), 0 );
#ifdef HOLYDBG_BE_ENDIAN
  const auto rbytes = reinterpret_cast<const std::uint8_t *>(raddr_p.first);
  const int disp = buf.size() - sizeof(T);
  assert( disp > 0 );
  std::memcpy(buf.data(), rbytes + disp, raddr_p.second);
#else  // HOLYDBG_BE_ENDIAN
  std::memcpy(buf.data(), raddr_p.first, raddr_p.second);
#endif // HOLYDBG_BE_ENDIAN
  return native_load<T>(buf.data());
}

template <typename T> inline
T ThreadContext::get_reg(unsigned int reg_idx) const
{
  return reg_value<T>(reg_idx);
}

template <typename T>
void ThreadContext::set_reg(unsigned int reg_idx, const T & value)
{
  static_assert( is_valid_reg_value<T>(), "invalid reg value type" );
  
  if(raw_ctx_) {
    const auto raddr_p = raw_ctx_->reg_addr(reg_idx);
    std::vector<std::uint8_t> buf( std::max(raddr_p.second, sizeof(T)), 0 );
#ifdef HOLYDBG_BE_ENDIAN
    const auto vbytes = reinterpret_cast<std::uint8_t *>(buf.data());
    const int disp = buf.size() - sizeof(T);
    assert( disp > 0 );
    be_store(vbytes + disp, value);
    std::memcpy(raddr_p.first, vbytes + disp, raddr_p.second);
#else  // HOLYDBG_BE_ENDIAN
    le_store(buf.data(), value);
    std::memcpy(raddr_p.first, buf.data(), raddr_p.second);
#endif // HOLYDBG_BE_ENDIAN
  }
}

} // namespace hdbg

#endif // __HOLYDBG_DBG_THREAD_CONTEXT_HPP__
