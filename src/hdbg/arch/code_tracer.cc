#include <hdbg/arch/code_tracer.hpp>

namespace hdbg {

const void * CodeTracer::root_block() const
{
  return const_cast<const CodeTracer *>(this)->root_block();
}

const void * CodeTracer::get_block(std::uintptr_t addr) const
{
  return const_cast<const CodeTracer *>(this)->get_block(addr);
}

} // namespace hdbg
