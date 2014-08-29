#ifndef __HDBG_ARCH_CODE_TRACER_HPP__
#define __HDBG_ARCH_CODE_TRACER_HPP__

#include <hdbg/config.h>

#include <cstddef>
#include <cstdint>

namespace hdbg {

class HDBG_API CodeTracer
{
public:
  virtual ~CodeTracer() = default;
  virtual void * root_block() const = 0;
  
  virtual void * add_block(std::uintptr_t from, std::uintptr_t to) = 0;
  virtual void * remove_block(void * block) = 0;
  virtual void link_block(void * parent, void * child) = 0;
  
  virtual void * get_block(std::uintptr_t addr) const = 0;
  virtual std::uintptr_t block_start(const void * block) const = 0;
  virtual std::size_t block_size(const void * block) const = 0;
};

} // namespace hdbg

#endif // __HDBG_ARCH_CODE_TRACER_HPP__
