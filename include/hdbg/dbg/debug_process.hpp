#ifndef __HDBG_DBG_DEBUG_PROCESS_HPP__
#define __HDBG_DBG_DEBUG_PROCESS_HPP__

#include <hdbg/config.h>
#include <hdbg/sys_types.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>

namespace hdbg {

class BinaryFormat;

class HDBG_API DebugProcess
{
public:
  
  enum OpenFlags
  {
    ProcKill = 1 << 0,
    MemRead  = 1 << 1,
    MemWrite = 1 << 2,
    
    AllAccess = ProcKill | MemRead | MemWrite
  };
  
  virtual ~DebugProcess() = default;
  
  virtual process_id id() const = 0;
  virtual std::uintptr_t image_base() const = 0;
  virtual const BinaryFormat & image() const = 0;
  
  virtual void kill(bool force = false) = 0;
  
  virtual std::size_t read_mem(std::uintptr_t from, std::size_t len, void * to) const = 0;
  virtual std::size_t write_mem(std::uintptr_t where, std::size_t len, const void * what) = 0;
};

} // namespace hdbg

#endif // __HDBG_DBG_DEBUG_PROCESS_HPP__
