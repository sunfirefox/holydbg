#ifndef __HOLYDBG_BINARY_FORMAT_HPP__
#define __HOLYDBG_BINARY_FORMAT_HPP__

#include <holydbg/arch_services.hpp>

#include <cstdint>

namespace hdbg {

class HDBG_EXPORT BinaryFormat
{
public:
  virtual ~BinaryFormat() = default;
  virtual const char * arch() const = 0;
  virtual std::uintptr_t entry() const = 0;
};

} // namespace hdbg

#endif // __HOLYDBG_BINARY_FORMAT_HPP__
