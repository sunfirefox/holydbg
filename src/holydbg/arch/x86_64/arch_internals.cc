#include "arch_internals.hpp"

#include "raw_context.hpp"

namespace hdbg {

X64_ArchInternals::X64_ArchInternals() = default;
X64_ArchInternals::~X64_ArchInternals() = default;

std::unique_ptr<RawContext> X64_ArchInternals::make_raw_context() const
{
  return std::unique_ptr<RawContext>( new X64_RawContext );
}

const std::vector<std::uint8_t>& X64_ArchInternals::make_sw_bp() const
{
  static const std::vector<std::uint8_t> sw_break { 0xcc }; // __asm int3
  return sw_break;
}

} // namespace hdbg
