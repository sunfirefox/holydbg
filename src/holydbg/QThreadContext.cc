#include "QThreadContext.hpp"

namespace holydbg {

QThreadContext::QThreadContext(QObject * parent)
  : QHdbgObject( &thr_ctx_, parent ) {}

QThreadContext::~QThreadContext() = default;

QVariant QThreadContext::getReg(unsigned int reg_idx) const
{
  QVariant rv;
  try {
    rv.setValue( thr_ctx_.reg_value<std::uintptr_t>(reg_idx) );
  } catch(const std::out_of_range &) {}
  return rv;
}

void QThreadContext::setReg(unsigned int reg_idx, QVariant reg_value)
{
  const auto rv = reg_value.value<std::uintptr_t>();
  try {
    thr_ctx_.set_reg(reg_idx, rv);
  } catch(const std::out_of_range &) {}
}

} // namespace holydbg

#include "moc_QThreadContext.cpp"
