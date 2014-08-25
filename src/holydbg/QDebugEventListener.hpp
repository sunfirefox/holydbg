#ifndef __HOLYDBG_QDEBUGEVENTLISTENER_HPP__
#define __HOLYDBG_QDEBUGEVENTLISTENER_HPP__

#include "QHdbgObject.hpp"

#include <hdbg/dbg/debuggee.hpp>
#include <hdbg/dbg/debug_event.hpp>

#include <memory>

namespace holydbg {

class QDebuggee;
class QDebugThread;

class QDebugEventListener final
  : public QHdbgObject<std::shared_ptr<hdbg::DebugEventListener>>
{
  Q_OBJECT
  
public:
  QDebugEventListener(QObject * parent = nullptr);
  virtual ~QDebugEventListener();
  
signals:
  void processCreated(QDebuggee *, QDebugThread *);
  void processExited(QDebuggee *, int exit_code);
  void processKilled(QDebuggee *, int term_sig);
  
  void threadCreated(QDebuggee *, QDebugThread *, QDebugThread *);
  void threadExited(QDebuggee *, QDebugThread *, int exit_code);
  
  void breakpointHit(QDebuggee *, QDebugThread *);
  void singlestep(QDebuggee *, QDebugThread *);
  
  void accessViolation(QDebuggee *, QDebugThread *);
  void floatingPointException(QDebuggee *, QDebugThread *);
  void illegalInstruction(QDebuggee *, QDebugThread *);
  
private:
  std::shared_ptr<hdbg::DebugEventListener> listener_;
};

} // namespace holydbg

Q_DECLARE_METATYPE(holydbg::QDebugEventListener *)

#endif // __HOLYDBG_QDEBUGEVENTLISTENER_HPP__
