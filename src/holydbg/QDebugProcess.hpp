#ifndef __HOLYDBG_QDEBUGPROCESS_HPP__
#define __HOLYDBG_QDEBUGPROCESS_HPP__

#include "QHdbgObject.hpp"

#include <hdbg/dbg/debug_process.hpp>

#include <QByteArray>

#include <cstdint>

namespace holydbg {

class QDebuggee;

class QDebugProcess final
  : public QHdbgObject<hdbg::DebugProcess>
{
  Q_OBJECT
  Q_PROPERTY(QDebuggee * debuggee READ debuggee CONSTANT)
  Q_PROPERTY(hdbg::process_id id READ id CONSTANT)
  Q_PROPERTY(std::uintptr_t imageBase READ imageBase CONSTANT)
  
public:
  explicit QDebugProcess(QDebuggee * parent, hdbg::DebugProcess * dbg_proc);
  virtual ~QDebugProcess();
  
  QDebuggee * debuggee();
  
  hdbg::process_id id() const;
  std::uintptr_t imageBase() const;
  
  Q_INVOKABLE QByteArray readMem(std::uintptr_t addr, std::size_t len) const;
  Q_INVOKABLE std::size_t writeMem(std::uintptr_t addr, const QByteArray & data);
  
  Q_INVOKABLE void kill(bool force = false);
  
private:
  hdbg::DebugProcess * dbg_proc_;
  QDebuggee * qdebuggee_;
};

} // namespace holydbg

Q_DECLARE_METATYPE(holydbg::QDebugProcess *)

#endif // __HOLYDBG_QDEBUGPROCESS_HPP__
