#ifndef __HOLYDBG_ARCH_QX64_HPP__
#define __HOLYDBG_ARCH_QX64_HPP__

#include <QObject>

namespace holydbg {

class QX64 final
  : public QObject
{
  Q_OBJECT
  Q_ENUMS(RegIndex)
  
public:
  QX64();
  virtual ~QX64();
  
  enum RegIndex
  {
    // gen purpose
    Rax, Rbx,    Rcx, Rdx,
    Rsi, Rdi,    Rbp, Rsp,
    R8,  R9,     R10, R11,
    R12, R13,    R14, R15,
    Rip, Rflags, Cs,  Ss,
    Ds,  Es,     Fs,  Gs,
    
    // fpu regs
    St0, St1, St2, St3,
    St4, St5, St6, St7,
    
    // debug regs
    Dr0, Dr1, Dr2, Dr3,
    Dr4, Dr5, Dr6, Dr7,
  };
};

} // namespace holydbg

#endif // __HOLYDBG_ARCH_QX86_64_HPP__
