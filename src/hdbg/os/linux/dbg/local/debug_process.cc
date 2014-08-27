#include <hdbg/dbg/local/debug_process.hpp>

#include <hdbg/binfmt/from_image.hpp>
#include <hdbg/enum/enum_mempages.hpp>

#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <memory>
#include <system_error>
#include <sstream>
#include <string>
#include <utility>

#include <iostream>

namespace hdbg {

namespace {

std::string proc_mem_path(process_id pid)
{
  std::ostringstream oss;
  oss << "/proc/" << pid << "/mem";
  return oss.str();
}

int open_mem_flags(int flags)
{
  int op_mode;
  if(flags & DebugProcess::OpenFlags::MemRead) {
    if(flags & DebugProcess::OpenFlags::MemWrite)
      op_mode = O_RDWR;
    else   // !DebugProcess::OpenFlags::MemWrite
      op_mode = O_RDONLY;
  } else { // !DebugProcess::OpenFlags::MemRead
    if(flags & DebugProcess::OpenFlags::MemWrite)
      op_mode = O_WRONLY;
    else   // !DebugProcess::OpenFlags::MemWrite
      op_mode = 0;
  }
  return op_mode | O_CLOEXEC | O_LARGEFILE;
}

} // namespace

struct LocalDebugProcess::Impl
{
  Impl(process_id pid, int dbg_flags);
  ~Impl();
  
  const process_id pid_;
  const std::uintptr_t base_addr_;
  const int fdmem_;
  std::unique_ptr<BinaryFormat> image_;
};

LocalDebugProcess::Impl::Impl(process_id proc_id, int flags)
  : pid_( proc_id )
  , base_addr_( enum_mempages(proc_id).begin()->base )
  , fdmem_([proc_id, flags] {
      const std::string mem_path = proc_mem_path(proc_id);
      const int op_mem_flags = open_mem_flags(flags);
      const int fdmem = ::open(mem_path.c_str(), op_mem_flags);
      if(fdmem == -1)
        throw std::system_error(errno, std::system_category());
      return fdmem;
    }() ) {}

LocalDebugProcess::Impl::~Impl()
{
  ::close(fdmem_);
}

LocalDebugProcess::LocalDebugProcess(process_id pid, int flags)
  : pimpl_( new Impl(pid, flags) ) {}

LocalDebugProcess::LocalDebugProcess(LocalDebugProcess &&) = default;
LocalDebugProcess::~LocalDebugProcess() = default;

LocalDebugProcess & LocalDebugProcess::operator=(LocalDebugProcess &&) = default;

process_id LocalDebugProcess::id() const
{
  return pimpl_->pid_;
}

std::uintptr_t LocalDebugProcess::image_base() const
{
  return pimpl_->base_addr_;
}

const BinaryFormat & LocalDebugProcess::image() const
{
  if(!pimpl_->image_)
    pimpl_->image_ = binfmt_from_image(*this, pimpl_->base_addr_);
  return *pimpl_->image_;
}

void LocalDebugProcess::kill(bool force)
{
  if(::kill(pimpl_->pid_, force ? SIGKILL : SIGTERM) == -1)
    throw std::system_error(errno, std::system_category());
}

std::size_t LocalDebugProcess::read_mem(std::uintptr_t from, std::size_t len, void * to) const
{
  const ssize_t read = ::pread(pimpl_->fdmem_, to, len, static_cast<off_t>(from));
  if(read == -1)
    throw std::system_error(errno, std::system_category());
  return read;
}

std::size_t LocalDebugProcess::write_mem(std::uintptr_t where, std::size_t len, const void * what)
{
  const ssize_t written = ::pwrite(pimpl_->fdmem_, what, len, static_cast<off_t>(where));
  if(written == -1)
    throw std::system_error(errno, std::system_category());
  return written;
}

void * LocalDebugProcess::nativeHandle()
{
  return nullptr;
}

} // namespace hdbg
