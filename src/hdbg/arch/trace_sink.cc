#include <hdbg/arch/trace_sink.hpp>

namespace hdbg {

const void * TraceSink::root_block() const
{
  return const_cast<const TraceSink *>(this)->root_block();
}

const void * TraceSink::get_block(std::uintptr_t addr) const
{
  return const_cast<const TraceSink *>(this)->get_block(addr);
}

} // namespace hdbg
