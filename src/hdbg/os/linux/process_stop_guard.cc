#include "process_stop_guard.hpp"

#include <signal.h>
#include <sys/wait.h>

#include <system_error>

namespace hdbg {

ProcessStopGuard::ProcessStopGuard(process_id pid)
  : pid_(pid)
{
  if(::kill(pid_, SIGSTOP) == -1)
    throw std::system_error(errno, std::system_category());
  
  if(::waitpid(pid_, nullptr, WUNTRACED) == -1)
    throw std::system_error(errno, std::system_category());
}

ProcessStopGuard::ProcessStopGuard(ProcessStopGuard && ps_guard)
  : pid_( ps_guard.pid_ )
{
  ps_guard.pid_ = 0;
}

ProcessStopGuard::~ProcessStopGuard()
{
  if(pid_ == 0)
    return;
  
  if(::kill(pid_, SIGCONT) == -1)
    throw std::system_error(errno, std::system_category());
  
  if(::waitpid(pid_, nullptr, WCONTINUED) == -1)
    throw std::system_error(errno, std::system_category());
}

} // namespace hdbg
