#ifndef __HOLYDBG_QTHREADCONTEXT_HPP__
#define __HOLYDBG_QTHREADCONTEXT_HPP__

#include "QHdbgObject.hpp"

#include <hdbg/arch_services.hpp>
#include <hdbg/dbg/debug_thread.hpp>
#include <hdbg/dbg/thread_context.hpp>

#include <QString>
#include <QVariant>

namespace holydbg {

class QThreadContext final
  : public QHdbgObject<hdbg::ThreadContext>
{
  Q_OBJECT
  
public:
  QThreadContext(QObject * parent = nullptr);
  virtual ~QThreadContext();
  
  Q_INVOKABLE QVariant getReg(unsigned int reg_idx) const;
  Q_INVOKABLE void setReg(unsigned int reg_idx, QVariant reg_value);
  
private:
  hdbg::ThreadContext thr_ctx_;
};

} // namespace holydbg

Q_DECLARE_METATYPE(holydbg::QThreadContext *)

#endif // __HOLYDBG_QTHREADCONTEXT_HPP__
