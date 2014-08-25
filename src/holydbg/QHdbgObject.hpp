#ifndef __HOLYDBG_QHDBGOBJECT_HPP__
#define __HOLYDBG_QHDBGOBJECT_HPP__

#include <QObject>
#include <QMap>

#include <type_traits>

namespace holydbg {

template <class T>
class QHdbgObject
  : public QObject
{
  template <typename QHdbgT>
  static constexpr bool is_valid_type()
  {
    return std::is_base_of<QHdbgObject<T>, QHdbgT>::value;
  }
  
public:
  template <typename QHdbgT>
  static QHdbgT * translate(T * );
  
  QHdbgObject(T * hdbg_obj, QObject * parent = nullptr);
  virtual ~QHdbgObject();
  
  T * hdbgObject();
  const T * hdbgObject() const;
  
private:
  static QMap<T *, QHdbgObject *> tr_table_;
  T * hdbg_obj_ = nullptr;
};

template <class QHdbgT, typename T> inline
QHdbgT * qhdbgTranslateObject(T * hdbg_obj)
{
  return QHdbgT::template translate<QHdbgT>(hdbg_obj);
}

template <class T>
QMap<T *, QHdbgObject<T> *> QHdbgObject<T>::tr_table_;

template <class T> template <class QHdbgT>
QHdbgT * QHdbgObject<T>::translate(T * hdbg_obj)
{
  static_assert(is_valid_type<QHdbgT>(), "invalid QHdbgObject type");
  return static_cast<QHdbgT *>( tr_table_.value(hdbg_obj, nullptr) );
}

template <class T>
QHdbgObject<T>::QHdbgObject(T * hdbg_obj, QObject * parent)
  : QObject( parent )
  , hdbg_obj_( hdbg_obj )
{
  tr_table_.insert(hdbg_obj_, this);
}

template <class T>
QHdbgObject<T>::~QHdbgObject()
{
  tr_table_.remove(hdbg_obj_);
}

template <class T>
T * QHdbgObject<T>::hdbgObject()
{
  return hdbg_obj_;
}

template <class T>
const T * QHdbgObject<T>::hdbgObject() const
{
  return hdbg_obj_;
}

} // namespace holydbg

#endif // __HOLYDBG_QHDBGOBJECT_HPP__
