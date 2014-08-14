#ifndef __HOLYDBG_SYS_TYPES__
#define __HOLYDBG_SYS_TYPES__

#include <holydbg/config.h>

#if defined( HOLYDBG_OS_LINUX )
#  include <unistd.h>
#elif defined( HOLYDBG_OS_WINDOWS )
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#else
#  error unsupported os
#endif

namespace hdbg {

#if defined( HOLYDBG_OS_LINUX )
  typedef pid_t process_id;
  typedef pid_t thread_id;
#elif defined( HOLYDBG_OS_WINDOWS )
  typedef DWORD process_id;
  typedef DWORD thread_id;
#endif

} // namespace holydbg

#endif // __HOLYDBG_SYS_TYPES__
