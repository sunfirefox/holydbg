#ifndef __HOLYDBG_SRC_ARCH_ARCH_INTERNALS_HPP__
#define __HOLYDBG_SRC_ARCH_ARCH_INTERNALS_HPP__

#include <holydbg/arch/raw_context.hpp>

#include <cstdint>
#include <memory>
#include <vector>

namespace hdbg {

class ArchInternals
{
public:
  virtual ~ArchInternals() = default;
  virtual std::unique_ptr<RawContext> make_raw_context() const = 0;
  virtual const std::vector<std::uint8_t> & make_sw_bp() const = 0;
};

} // namespace hdbg

#endif // __HOLYDBG_SRC_ARCH_ARCH_INTERNALS_HPP__
