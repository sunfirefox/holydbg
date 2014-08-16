#ifndef __HDBG_SRC_OS_LINUX_PROCESS_STOP_GUARD_HPP__
#define __HDBG_SRC_OS_LINUX_PROCESS_STOP_GUARD_HPP__

#include <hdbg/sys_types.hpp>

namespace hdbg {

class ProcessStopGuard
{
public:
  ProcessStopGuard(process_id pid);
  ProcessStopGuard(const ProcessStopGuard &) = delete;
  ProcessStopGuard(ProcessStopGuard &&);
  ~ProcessStopGuard();
  
private:
  process_id pid_;
};

} // namespace hdbg

#endif // __HDBG_SRC_OS_LINUX_PROCESS_STOP_GUARD_HPP__
