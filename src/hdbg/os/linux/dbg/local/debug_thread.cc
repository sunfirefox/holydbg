#include <hdbg/dbg/local/debug_thread.hpp>

#include "pt_runner.hpp"

#include <hdbg/dbg/thread_context.hpp>
#include <hdbg/dbg/local/debug_process.hpp>

namespace hdbg {

struct LocalDebugThread::Impl
{
  thread_id tid_;
  LocalDebugProcess * process_;
};

LocalDebugThread::LocalDebugThread(LocalDebugProcess & process, thread_id tid, int flags)
  : pimpl_( new Impl )
{
  pimpl_->process_ = &process;
  pimpl_->tid_ = tid;
}

LocalDebugThread::LocalDebugThread(LocalDebugThread &&) = default;
LocalDebugThread::~LocalDebugThread() = default;

LocalDebugThread & LocalDebugThread::operator=(LocalDebugThread &&) = default;

LocalDebugProcess & LocalDebugThread::process()
{
  return *pimpl_->process_;
}

const LocalDebugProcess & LocalDebugThread::process() const
{
  return *pimpl_->process_;
}

thread_id LocalDebugThread::id() const
{
  return pimpl_->tid_;
}

void LocalDebugThread::get_context(ThreadContext & thr_ctx) const
{
  pt_runner.run([this, &thr_ctx] {
    thr_ctx.obtain_from(*this);
  });
}

void LocalDebugThread::set_context(const ThreadContext & thr_ctx)
{
  pt_runner.run([this, &thr_ctx] {
    thr_ctx.apply_to(*this);
  });
}

void * LocalDebugThread::nativeHandle()
{
  return nullptr;
}

} // namespace hdbg
