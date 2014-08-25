#ifndef __HOLYDBG_QBREAKPOINT_HPP__
#define __HOLYDBG_QBREAKPOINT_HPP__

#include "QHdbgObject.hpp"

#include <hdbg/dbg/debuggee.hpp>

#include <hdbg/dbg/breakpoint.hpp>
#include <hdbg/dbg/bps/sw_breakpoint.hpp>
#include <hdbg/dbg/bps/hw_breakpoint.hpp>

#include <memory>

namespace holydbg {

class QDebuggee;
class QDebugThread;
class QThreadContext;

class QBreakpoint
  : public QHdbgObject<hdbg::Breakpoint>
{
  Q_OBJECT
  
public:
  virtual ~QBreakpoint();
  
signals:
  void triggered(QDebuggee *, QDebugThread *, QThreadContext *, hdbg::breakpoint_id);
  
protected:
  QBreakpoint(std::unique_ptr<hdbg::Breakpoint> && bp);
  
private:
  std::unique_ptr<hdbg::Breakpoint> bp_;
};

template <class Bp>
class QBreakpointT final
  : public QBreakpoint
{
public:
  template <typename... Args> QBreakpointT(Args&&...);
  virtual ~QBreakpointT();
};

typedef QBreakpointT<hdbg::SwBreakpoint> QSwBreakpoint;
typedef QBreakpointT<hdbg::HwBreakpoint> QHwBreakpoint;

template <class Bp> template <typename... Args>
QBreakpointT<Bp>::QBreakpointT(Args&&... args)
  : QBreakpoint( new Bp( std::forward<Args>(args)... )) {}

template <class Bp>
QBreakpointT<Bp>::~QBreakpointT() = default;

} // namespace holydbg

Q_DECLARE_METATYPE(holydbg::QBreakpoint *)

#endif // __HOLYDBG_QBREAKPOINT_HPP__
