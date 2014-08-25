#ifndef __HOLYDBG_QDEBUGGEE_HPP__
#define __HOLYDBG_QDEBUGGEE_HPP__

#include "QHdbgObject.hpp"

#include <hdbg/dbg/debuggee.hpp>

namespace holydbg {

class QBreakpoint;
class QDebugProcess;
class QDebugThread;
class QThreadContext;
class QDebugEventListener;

class QDebuggee final
  : public QHdbgObject<hdbg::Debuggee>
{
  Q_OBJECT
  Q_PROPERTY(bool attached READ isAttached)
  Q_PROPERTY(QDebugProcess * process READ process CONSTANT)
  
public:
  QDebuggee(std::unique_ptr<hdbg::Debuggee> && debuggee);
  virtual ~QDebuggee();
  
  bool isAttached() const;
  
  QDebugProcess * process();
  
  Q_INVOKABLE QDebugThread * getThread(hdbg::thread_id tid);
  
  Q_INVOKABLE void singlestep(QDebugThread *);
  Q_INVOKABLE void runUntilNextEvent();
  Q_INVOKABLE void run();
  
  Q_INVOKABLE void kill();
  Q_INVOKABLE void detach();
  
  Q_INVOKABLE void addListener(QDebugEventListener *);
  Q_INVOKABLE void removeListener(QDebugEventListener *);
  Q_INVOKABLE void discardEvent();
  
  Q_INVOKABLE hdbg::breakpoint_id setBp(QBreakpoint * bp);
  Q_INVOKABLE void removeBp(hdbg::breakpoint_id bp_id);
  Q_INVOKABLE void removeAllBps();
  
  Q_INVOKABLE QDebuggee * attachChild(hdbg::process_id pid);
  
private:
  std::unique_ptr<hdbg::Debuggee> debuggee_;
  QDebugProcess * qproc_;
  QThreadContext * qthr_ctx_;
};

} // namespace holydbg

Q_DECLARE_METATYPE(holydbg::QDebuggee *)

#endif // __HOLYDBG_QDEBUGGEE_HPP__
