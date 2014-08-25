#ifndef __HOLYDBG_QDEBUGTHREAD_HPP__
#define __HOLYDBG_QDEBUGTHREAD_HPP__

#include "QHdbgObject.hpp"

#include <hdbg/dbg/debug_thread.hpp>

namespace holydbg {

class QDebuggee;
class QDebugProcess;
class QThreadContext;

class QDebugThread final
  : public QHdbgObject<hdbg::DebugThread>
{
  Q_OBJECT
  Q_PROPERTY(QDebugProcess * process READ process CONSTANT)
  Q_PROPERTY(hdbg::thread_id id READ id CONSTANT)
  
public:
  QDebugThread(QDebuggee * parent, hdbg::DebugThread * dbg_thr);
  virtual ~QDebugThread();
  
  QDebugProcess * process();
  hdbg::thread_id id() const;
  
  Q_INVOKABLE void getContext(QThreadContext *) const;
  Q_INVOKABLE void setContext(const QThreadContext * qthr_ctx);
  
private:
  hdbg::DebugThread * dbg_thr_ = nullptr;
  QDebuggee * qdebuggee_ = nullptr;
};

} // namespace holydbg

Q_DECLARE_METATYPE(holydbg::QDebugThread *)

#endif // __HOLYDBG_QDEBUGTHREAD_HPP__
