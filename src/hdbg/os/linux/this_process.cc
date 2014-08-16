#include <hdbg/this_process.hpp>

#include <unistd.h>

namespace hdbg { namespace this_process {

process_id id()
{
  return ::getpid();
}

} } // namespace hdbg::this_process
