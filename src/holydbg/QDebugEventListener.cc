#include "QDebugEventListener.hpp"

#include "QDebuggee.hpp"
#include "QDebugProcess.hpp"
#include "QDebugThread.hpp"

namespace holydbg {

class DebugEventForwarder
  : public hdbg::DebugEventListener
{
public:
  DebugEventForwarder(QDebugEventListener *);
  virtual ~DebugEventForwarder();
  
private:
  virtual void handle_event(const hdbg::ProcessCreatedEvent &) override;
  virtual void handle_event(const hdbg::ProcessExitedEvent &) override;
  virtual void handle_event(const hdbg::ProcessKilledEvent &) override;
  virtual void handle_event(const hdbg::ThreadCreatedEvent &) override;
  virtual void handle_event(const hdbg::ThreadExitedEvent &) override;
  virtual void handle_event(const hdbg::BreakpointHitEvent &) override;
  virtual void handle_event(const hdbg::SinglestepEvent &) override;
  virtual void handle_event(const hdbg::AccessViolationEvent &) override;
  virtual void handle_event(const hdbg::FloatingPointExceptionEvent &) override;
  virtual void handle_event(const hdbg::IllegalInstructionEvent &) override;
  
  QDebugEventListener * qlistener_ = nullptr;
};

DebugEventForwarder::DebugEventForwarder(QDebugEventListener * listener)
  : qlistener_( listener )
{
  Q_ASSERT(qlistener_);
}

DebugEventForwarder::~DebugEventForwarder() = default;

QDebugEventListener::QDebugEventListener(QObject * parent)
  : QHdbgObject( &listener_, parent )
  , listener_( std::make_shared<DebugEventForwarder>(this) ) {}

QDebugEventListener::~QDebugEventListener() = default;

void DebugEventForwarder::handle_event(const hdbg::ProcessCreatedEvent & evt)
{
  const auto qdebuggee = qhdbgTranslateObject<QDebuggee>(evt.debuggee);
  const auto qthread = qhdbgTranslateObject<QDebugThread>(evt.thread);
  if(qdebuggee && qthread)
    emit qlistener_->processCreated(qdebuggee, qthread);
}

void DebugEventForwarder::handle_event(const hdbg::ProcessExitedEvent & evt)
{
  const auto qdebuggee = qhdbgTranslateObject<QDebuggee>(evt.debuggee);
  if(qdebuggee)
    emit qlistener_->processExited(qdebuggee, evt.exit_code);
}

void DebugEventForwarder::handle_event(const hdbg::ProcessKilledEvent & evt)
{
  const auto qdebuggee = qhdbgTranslateObject<QDebuggee>(evt.debuggee);
  if(qdebuggee)
    emit qlistener_->processKilled(qdebuggee, evt.term_sig);
}

void DebugEventForwarder::handle_event(const hdbg::ThreadCreatedEvent & evt)
{
  const auto qdebuggee = qhdbgTranslateObject<QDebuggee>(evt.debuggee);
  const auto qthread = qhdbgTranslateObject<QDebugThread>(evt.thread);
  const auto qnew_thr = qhdbgTranslateObject<QDebugThread>(evt.new_thread);
  if(qdebuggee && qthread && qnew_thr)
    emit qlistener_->threadCreated(qdebuggee, qthread, qnew_thr);
}

void DebugEventForwarder::handle_event(const hdbg::ThreadExitedEvent & evt)
{
  const auto qdebuggee = qhdbgTranslateObject<QDebuggee>(evt.debuggee);
  const auto qthread = qhdbgTranslateObject<QDebugThread>(evt.thread);
  if(qdebuggee && qthread)
    emit qlistener_->threadExited(qdebuggee, qthread, evt.exit_code);
}

void DebugEventForwarder::handle_event(const hdbg::BreakpointHitEvent & evt)
{
  const auto qdebuggee = qhdbgTranslateObject<QDebuggee>(evt.debuggee);
  const auto qthread = qhdbgTranslateObject<QDebugThread>(evt.thread);
  if(qdebuggee && qthread)
    emit qlistener_->breakpointHit(qdebuggee, qthread);
}

void DebugEventForwarder::handle_event(const hdbg::SinglestepEvent & evt)
{
  const auto qdebuggee = qhdbgTranslateObject<QDebuggee>(evt.debuggee);
  const auto qthread = qhdbgTranslateObject<QDebugThread>(evt.thread);
  if(qdebuggee && qthread)
    emit qlistener_->singlestep(qdebuggee, qthread);
}

void DebugEventForwarder::handle_event(const hdbg::AccessViolationEvent & evt)
{
  const auto qdebuggee = qhdbgTranslateObject<QDebuggee>(evt.debuggee);
  const auto qthread = qhdbgTranslateObject<QDebugThread>(evt.thread);
  if(qdebuggee && qthread)
    emit qlistener_->accessViolation(qdebuggee, qthread);
}

void DebugEventForwarder::handle_event(const hdbg::FloatingPointExceptionEvent & evt)
{
  const auto qdebuggee = qhdbgTranslateObject<QDebuggee>(evt.debuggee);
  const auto qthread = qhdbgTranslateObject<QDebugThread>(evt.thread);
  if(qdebuggee && qthread)
    emit qlistener_->floatingPointException(qdebuggee, qthread);
}

void DebugEventForwarder::handle_event(const hdbg::IllegalInstructionEvent & evt)
{
  const auto qdebuggee = qhdbgTranslateObject<QDebuggee>(evt.debuggee);
  const auto qthread = qhdbgTranslateObject<QDebugThread>(evt.thread);
  if(qdebuggee && qthread)
    emit qlistener_->illegalInstruction(qdebuggee, qthread);
}

} // namespace holydbg

#include "moc_QDebugEventListener.cpp"
