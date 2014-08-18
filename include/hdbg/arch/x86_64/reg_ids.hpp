#ifndef __HDBG_ARCH_X86_64_REG_IDS_HPP__
#define __HDBG_ARCH_X86_64_REG_IDS_HPP__

namespace hdbg {

enum {
  // gen purpose
  X64_RegRax, X64_RegRbx, X64_RegRcx, X64_RegRdx,
  X64_RegRsi, X64_RegRdi, X64_RegRbp, X64_RegRsp,
  X64_RegR8,  X64_RegR9,  X64_RegR10, X64_RegR11,
  X64_RegR12, X64_RegR13, X64_RegR14, X64_RegR15,
  X64_RegRip, X64_RegRflags, X64_SegCs, X64_SegSs,
  X64_SegDs,  X64_SegEs,  X64_SegFs,  X64_SegGs,
  
  // fpu regs
  X64_RegSt0, X64_RegSt1, X64_RegSt2, X64_RegSt3,
  X64_RegSt4, X64_RegSt5, X64_RegSt6, X64_RegSt7,
  
  // debug regs
  X64_RegDr0, X64_RegDr1, X64_RegDr2, X64_RegDr3,
  X64_RegDr4, X64_RegDr5, X64_RegDr6, X64_RegDr7
};

} // namespace hdbg

#endif // __HDBG_ARCH_X86_64_REG_IDS_HPP__
