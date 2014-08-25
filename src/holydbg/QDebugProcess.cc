#include "QDebugProcess.hpp"

#include "QDebuggee.hpp"

namespace holydbg {

QDebugProcess::QDebugProcess(QDebuggee * parent, hdbg::DebugProcess * dbg_proc)
  : QHdbgObject( dbg_proc, parent )
  , dbg_proc_( dbg_proc )
  , qdebuggee_( parent )
{
  Q_ASSERT(qdebuggee_);
  Q_ASSERT(dbg_proc_);
}

QDebugProcess::~QDebugProcess() = default;

QDebuggee * QDebugProcess::debuggee()
{
  return qdebuggee_;
}

hdbg::process_id QDebugProcess::id() const
{
  return dbg_proc_->id();
}

std::uintptr_t QDebugProcess::imageBase() const
{
  return dbg_proc_->image_base();
}

void QDebugProcess::kill(bool force)
{
  dbg_proc_->kill(force);
}

QByteArray QDebugProcess::readMem(std::uintptr_t addr, std::size_t len) const
{
  QByteArray buf;
  buf.resize(len);
  const auto read = dbg_proc_->read_mem(addr, len, buf.data());
  buf.resize(read);
  return buf;
}

std::size_t QDebugProcess::writeMem(std::uintptr_t addr, const QByteArray & data)
{
  return dbg_proc_->write_mem(addr, data.size(), data.constData());
}

} // namespace holydbg

#include "moc_QDebugProcess.cpp"
