#ifndef __HDBG_SRC_ARCH_ARCH_INTERNALS_HPP__
#define __HDBG_SRC_ARCH_ARCH_INTERNALS_HPP__

#include <hdbg/arch/raw_context.hpp>

#include <cstdint>
#include <memory>
#include <vector>

namespace hdbg {

class ThreadContext;

class ArchInternals
{
public:
  virtual ~ArchInternals() = default;
  virtual std::shared_ptr<RawContext> make_raw_context() const = 0;
  virtual const std::vector<std::uint8_t> & sw_bpx_template() const = 0;
  virtual const std::vector<unsigned int> & hw_bpx_reg_indexes() const = 0;
  virtual bool is_hw_bpx_reg_enabled(unsigned int reg_idx, const ThreadContext & thr_ctx) const = 0;
  virtual void set_hw_bpx_enabled(unsigned int reg_idx, bool enable, ThreadContext & thr_ctx) const = 0;
};

} // namespace hdbg

#endif // __HDBG_SRC_ARCH_ARCH_INTERNALS_HPP__
