#ifndef __HDBG_BINARY_FORMAT_HPP__
#define __HDBG_BINARY_FORMAT_HPP__

#include <hdbg/arch_services.hpp>

#include <cstdint>

namespace hdbg {

class HDBG_EXPORT BinaryFormat
{
public:
  virtual ~BinaryFormat() = default;
  virtual const char * arch() const = 0;
  virtual std::uintptr_t preferred_base() const = 0;
  virtual std::ptrdiff_t entry() const = 0;
};

} // namespace hdbg

#endif // __HDBG_BINARY_FORMAT_HPP__
