#include "QDebuggee.hpp"

#include "QBreakpoint.hpp"
#include "QDebugProcess.hpp"
#include "QDebugThread.hpp"
#include "QThreadContext.hpp"
#include "QDebugEventListener.hpp"

#include <utility>

namespace holydbg {

QDebuggee::QDebuggee(std::unique_ptr<hdbg::Debuggee> && debuggee)
  : QHdbgObject( debuggee.get() )
  , debuggee_( std::move(debuggee) )
  , qproc_( new QDebugProcess(this, &debuggee_->process()) )
  , qthr_ctx_( new QThreadContext(this) )
{
  Q_ASSERT(debuggee_);
  Q_ASSERT(qproc_);
}

QDebuggee::~QDebuggee() = default;

QDebugProcess * QDebuggee::process()
{
  Q_ASSERT( isAttached() );
  return qproc_;
}

QDebugThread * QDebuggee::getThread(hdbg::thread_id tid)
{
  Q_ASSERT( isAttached() );
  const auto dbg_thr = &debuggee_->get_thread(tid);
  const auto qthr = qhdbgTranslateObject<QDebugThread>(dbg_thr);
  return qthr ? qthr : new QDebugThread(this, dbg_thr);
}

bool QDebuggee::isAttached() const
{
  return debuggee_->attached();
}

void QDebuggee::singlestep(QDebugThread * qdbg_thr)
{
  Q_ASSERT( isAttached() );
  
  Q_ASSERT(qdbg_thr);
  const auto dbg_thr = qdbg_thr->hdbgObject();
  debuggee_->singlestep(*dbg_thr);
}

void QDebuggee::runUntilNextEvent()
{
  Q_ASSERT( isAttached() );
  debuggee_->run_until_next_event();
}

void QDebuggee::run()
{
  Q_ASSERT( isAttached() );
  debuggee_->run();
}

void QDebuggee::kill()
{
  Q_ASSERT( isAttached() );
  debuggee_->kill();
}

void QDebuggee::detach()
{
  Q_ASSERT( isAttached() );
  debuggee_->detach();
}

void QDebuggee::addListener(QDebugEventListener * qlistener)
{
  Q_ASSERT( isAttached() );
  
  Q_ASSERT( qlistener );
  const auto sp_listener = qlistener->hdbgObject();
  debuggee_->add_listener(*sp_listener);
}

void QDebuggee::removeListener(QDebugEventListener * qlistener)
{
  Q_ASSERT( isAttached() );
  
  Q_ASSERT(qlistener);
  const auto sp_listener = qlistener->hdbgObject();
  debuggee_->remove_listener(*sp_listener);
}

void QDebuggee::discardEvent()
{
  Q_ASSERT( isAttached() );
  debuggee_->discard_event();
}

hdbg::breakpoint_id QDebuggee::setBp(QBreakpoint * qbp)
{
  Q_ASSERT( isAttached() );
  
  Q_ASSERT(qbp);
  const auto bp = qbp->hdbgObject();
  return debuggee_->set_bp(bp,
    [this, qbp](hdbg::Debuggee & debuggee, hdbg::DebugThread & dbg_thr,
                hdbg::ThreadContext & , hdbg::breakpoint_id bp_id)
    {
      const auto qdebuggee = qhdbgTranslateObject<QDebuggee>(&debuggee);
      const auto qthread = qhdbgTranslateObject<QDebugThread>(&dbg_thr);
      if(qdebuggee && qthread)
        emit qbp->triggered(qdebuggee, qthread, qthr_ctx_, bp_id);
    });
}

void QDebuggee::removeBp(hdbg::breakpoint_id bp_id)
{
  Q_ASSERT( isAttached() );
  debuggee_->remove_bp(bp_id);
}

void QDebuggee::removeAllBps()
{
  Q_ASSERT( isAttached() );
  debuggee_->remove_all_bps();
}

QDebuggee * QDebuggee::attachChild(hdbg::process_id pid)
{
  return new QDebuggee( debuggee_->attach_child(pid) );
}

} // namespace holydbg

#include "moc_QDebuggee.cpp"
