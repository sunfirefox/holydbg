#include "process_kill_guard.hpp"

#include <signal.h>

#include <system_error>

namespace hdbg {

ProcessKillGuard::ProcessKillGuard(process_id pid, int sig)
  : pid_( pid ), sig_( sig ), doit_( true ) {}

ProcessKillGuard::ProcessKillGuard(ProcessKillGuard && pk_guard)
  : pid_( pk_guard.pid_ ), sig_( pk_guard.sig_ ) , doit_( pk_guard.doit_ )
{
  pk_guard.doit_ = false;
}

ProcessKillGuard::~ProcessKillGuard()
{
  if(!doit_)
    return;
  
  if(::kill(pid_, sig_) == -1)
    throw std::system_error(errno, std::system_category());
}

void ProcessKillGuard::cancel()
{
  doit_ = false;
}

} // namespace hdbg
