#ifndef __HOLYDBG_QHOLYDBGMODULE_HPP__
#define __HOLYDBG_QHOLYDBGMODULE_HPP__

#include <QObject>
#include <QString>

namespace holydbg {

class QHolyDbgModule final
  : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name CONSTANT)
  Q_PROPERTY(QString author READ author CONSTANT)
  Q_PROPERTY(QString version READ version CONSTANT)
  Q_PROPERTY(QString license READ license CONSTANT)
  
public:
  QHolyDbgModule(QObject * parent = nullptr);
  virtual ~QHolyDbgModule();
  
  QString name() const;
  QString author() const;
  QString version() const;
  QString license() const;
  
private:
  QString name_;
  QString author_;
  QString version_;
  QString license_;
};

} // namespace holydbg

#endif // __HOLYDBG_QHOLYDBGMODULE_HPP__
