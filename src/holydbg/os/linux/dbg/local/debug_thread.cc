#include <holydbg/dbg/local/debug_thread.hpp>

#include <holydbg/dbg/thread_context.hpp>
#include <holydbg/dbg/local/debug_process.hpp>

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

DebugProcess & LocalDebugThread::process()
{
  return *pimpl_->process_;
}

const DebugProcess & LocalDebugThread::process() const
{
  return *pimpl_->process_;
}

thread_id LocalDebugThread::id() const
{
  return pimpl_->tid_;
}

void LocalDebugThread::get_context(ThreadContext & thr_ctx) const
{
  thr_ctx.do_get_thread_context(*this);
}

void LocalDebugThread::set_context(const ThreadContext & thr_ctx)
{
  thr_ctx.do_set_thread_context(*this);
}

} // namespace hdbg
