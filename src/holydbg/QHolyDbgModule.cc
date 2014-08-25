#include "QHolyDbgModule.hpp"

namespace holydbg {

QHolyDbgModule::QHolyDbgModule(QObject * parent)
  : QObject( parent ) {}

QHolyDbgModule::~QHolyDbgModule() = default;

QString QHolyDbgModule::name() const
{
  return name_;
}

QString QHolyDbgModule::author() const
{
  return author_;
}

QString QHolyDbgModule::version() const
{
  return version_;
}

QString QHolyDbgModule::license() const
{
  return license_;
}

} // namespace holydbg

#include "moc_QHolyDbgModule.cpp"
