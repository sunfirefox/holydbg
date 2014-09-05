#ifndef __HDBG_HDBG_HPP__
#define __HDBG_HDBG_HPP__

#include <hdbg/hdbg_dll.hpp>

#define HOLYDBG_MAKE_VERSION(major, minor, patch) (((major) & 0xff) << 16 | \
                                                   ((minor) & 0xff) <<  8 | \
                                                   ((patch) & 0xff) <<  0 )

#define HOLYDBG_VERSION_MAJOR(version) (((version) >> 16) & 0xff)
#define HOLYDBG_VERSION_MINOR(version) (((version) >>  8) & 0xff)
#define HOLYDBG_VERSION_PATCH(version) (((version) >>  0) & 0xff)

#define LIBHDBG_VERSION HOLYDBG_MAKE_VERSION(0,3,0)

namespace hdbg {
  unsigned int get_version();
} // namespace hdbg

#endif // __HDBG_HDBG_HPP__
