#ifndef __HDBG_ARCH_DECODED_INST_HPP__
#define __HDBG_ARCH_DECODED_INST_HPP__

#include <cstddef>
#include <cstdint>

namespace hdbg {

struct DecodedInst
{
  enum {
    Conditional  = 1 << 0,
    CtrlTransfer = 1 << 1,
  };
  
  std::uintptr_t vaddr;
  std::size_t size;
  unsigned int flags;
  void * raw_info;
};

} // namespace hdbg

#endif // __HDBG_ARCH_DECODED_INST_HPP__
