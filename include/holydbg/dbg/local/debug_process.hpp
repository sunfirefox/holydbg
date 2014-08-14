#ifndef __HOLYDBG_DEBUG_PROCESS_HPP__
#define __HOLYDBG_DEBUG_PROCESS_HPP__

#include <holydbg/sys_types.hpp>
#include <holydbg/dbg/debug_process.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>

namespace hdbg {

class HDBG_EXPORT LocalDebugProcess final : public DebugProcess
{
public:
  LocalDebugProcess(process_id pid, int flags);
  LocalDebugProcess(const LocalDebugProcess &) = delete;
  LocalDebugProcess(LocalDebugProcess &&);
  virtual ~LocalDebugProcess();
  
  LocalDebugProcess & operator=(const LocalDebugProcess &) = delete;
  LocalDebugProcess & operator=(LocalDebugProcess &&);
  
  virtual process_id id() const override;
  virtual std::uintptr_t image_base() const override;
  
  virtual void kill(bool force) override;
  
  virtual std::size_t read_mem(std::uintptr_t from, std::size_t len, void * to) const override;
  virtual std::size_t write_mem(std::uintptr_t where, std::size_t len, const void * what) override;
  
private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

} // namespace hdbg

#endif // __HOLYDBG_DBG_LOCAL_DEBUG_PROCESS_HPP__
