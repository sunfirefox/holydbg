#ifndef __HDBG_SRC_OS_LINUX_PROCESS_KILL_GUARD_HPP__
#define __HDBG_SRC_OS_LINUX_PROCESS_KILL_GUARD_HPP__

#include <hdbg/sys_types.hpp>

namespace hdbg {

class ProcessKillGuard
{
public:
  ProcessKillGuard(process_id pid, int sig);
  ProcessKillGuard(const ProcessKillGuard &) = delete;
  ProcessKillGuard(ProcessKillGuard &&);
  ~ProcessKillGuard();
  
  void cancel();
  
private:
  process_id pid_;
  int sig_;
  bool doit_;
};

} // namespace hdbg

#endif // __HDBG_SRC_OS_LINUX_PROCESS_KILL_GUARD_HPP__
