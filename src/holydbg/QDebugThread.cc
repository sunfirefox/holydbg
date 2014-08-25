#include "QDebugThread.hpp"

#include "QDebuggee.hpp"
#include "QThreadContext.hpp"

namespace holydbg {

QDebugThread::QDebugThread(QDebuggee * parent, hdbg::DebugThread * dbg_thr)
  : QHdbgObject( dbg_thr, parent )
  , dbg_thr_( dbg_thr )
  , qdebuggee_( parent )
{
  Q_ASSERT(qdebuggee_);
  Q_ASSERT(dbg_thr_);
}

QDebugThread::~QDebugThread() = default;

QDebugProcess * QDebugThread::process()
{
  return qdebuggee_->process();
}

hdbg::thread_id QDebugThread::id() const
{
  return dbg_thr_->id();
}

void QDebugThread::getContext(QThreadContext * qthr_ctx) const
{
  Q_ASSERT(qthr_ctx);
  const auto thr_ctx = qthr_ctx->hdbgObject();
  dbg_thr_->get_context(*thr_ctx);
}

void QDebugThread::setContext(const QThreadContext * qthr_ctx)
{
  Q_ASSERT(qthr_ctx);
  const auto thr_ctx = qthr_ctx->hdbgObject();
  dbg_thr_->set_context(*thr_ctx);
}

} // namespace holydbg

#include "moc_QDebugThread.cpp"
