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
  
  virtual void * root_block() = 0;
  const void * root_block() const;
  
  virtual void * add_block(std::uintptr_t from, std::uintptr_t to) = 0;
  virtual void remove_block(void * block) = 0;
  virtual void link_block(void * block, void * with_block) = 0;
  
  virtual void * get_block(std::uintptr_t addr) = 0;
  const void * get_block(std::uintptr_t addr) const;
  
  virtual std::uintptr_t block_begin(const void * block) const = 0;
  virtual std::uintptr_t block_end(const void * block) const = 0;
};

} // namespace hdbg

#endif // __HDBG_ARCH_CODE_TRACER_HPP__
