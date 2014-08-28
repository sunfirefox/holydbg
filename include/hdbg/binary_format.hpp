#ifndef __HDBG_BINARY_FORMAT_HPP__
#define __HDBG_BINARY_FORMAT_HPP__

#include <hdbg/arch_services.hpp>

#include <cstdint>
#include <vector>

namespace hdbg {

class HDBG_API BinaryFormat
{
public:
  
  struct Section
  {
    enum Flags {
      None       = 0,
      Readable   = 1 << 0,
      Writable   = 1 << 1,
      Executable = 1 << 2,
    };
    
    std::uintptr_t raw_offset;
    std::size_t raw_size;
    std::ptrdiff_t vaddr;
    std::size_t vsize;
    unsigned int flags;
  };
  
  virtual ~BinaryFormat() = default;
  virtual const char * arch() const = 0;
  virtual std::uintptr_t preferred_base() const = 0;
  virtual std::ptrdiff_t entry_point() const = 0;
  virtual std::vector<Section> sections() const = 0;
  virtual std::vector<std::ptrdiff_t> code_entries() const = 0;
};

} // namespace hdbg

#endif // __HDBG_BINARY_FORMAT_HPP__
