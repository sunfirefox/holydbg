#include "../../../../../arch/intel/x86_64/raw_context.hpp"

#include <hdbg/dbg/thread_context.hpp>
#include <hdbg/dbg/local/debug_thread.hpp>

#include "../../../../../arch/intel/x86_64/reg_ids.hpp"

#include <sys/ptrace.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <system_error>

namespace hdbg {

X64_RawContext::X64_RawContext() = default;
X64_RawContext::X64_RawContext(const X64_RawContext &) = default;
X64_RawContext::X64_RawContext(X64_RawContext && raw_ctx) = default;
X64_RawContext::~X64_RawContext() = default;

bool X64_RawContext::valid_for(const char * arch) const
{
  return !std::strcmp(arch, "x86_64");
}

void X64_RawContext::obtain_from(const LocalDebugThread & dbg_thr)
{
  if(::ptrace(PTRACE_GETREGS, dbg_thr.id(), nullptr, &usr_regs_) == -1)
    throw std::system_error(errno, std::system_category());
  
  if(::ptrace(PTRACE_GETFPREGS, dbg_thr.id(), nullptr, &usr_fpregs_) == -1)
    throw std::system_error(errno, std::system_category());
  
  for(unsigned int i = 0; i < 8; ++i) {
    if(i != 4 && i != 5) {
      const unsigned int dreg_offs = offsetof(detail::x64_user, u_debugreg);
      const unsigned int dri_offs = dreg_offs + i * sizeof(detail::x64_user::u_debugreg[0]);
      errno = 0;
      const auto peekdata = ::ptrace(PTRACE_PEEKUSER, dbg_thr.id(), dri_offs, nullptr);
      if(peekdata == -1 && errno != 0)
        throw std::system_error(errno, std::system_category());
      u_debugreg_[i] = peekdata;
    } else {
      u_debugreg_[i] = 0;
    }
  }
}

void X64_RawContext::apply_to(LocalDebugThread & dbg_thr) const
{
  if(::ptrace(PTRACE_SETREGS, dbg_thr.id(), nullptr, &usr_regs_) == -1)
    throw std::system_error(errno, std::system_category());
  
  if(::ptrace(PTRACE_SETFPREGS, dbg_thr.id(), nullptr, &usr_fpregs_) == -1)
    throw std::system_error(errno, std::system_category());
  
  for(unsigned int i = 0; i < 8; ++i) {
    if(i != 4 && i != 5) {
      const unsigned int dreg_offs = offsetof(detail::x64_user, u_debugreg);
      const unsigned int dri_offs = dreg_offs + i * sizeof(detail::x64_user::u_debugreg[0]);
      if(::ptrace(PTRACE_POKEUSER, dbg_thr.id(), dri_offs, u_debugreg_[i]) == -1)
        throw std::system_error(errno, std::system_category());
    }
  }
}

std::pair<const void *, std::size_t> X64_RawContext::reg_addr(unsigned int reg_idx) const
{
  switch(reg_idx) {
    case X64_RegRax: return std::make_pair(&usr_regs_.rax, 8);
    case X64_RegRbx: return std::make_pair(&usr_regs_.rbx, 8);
    case X64_RegRcx: return std::make_pair(&usr_regs_.rcx, 8);
    case X64_RegRdx: return std::make_pair(&usr_regs_.rdx, 8);
    case X64_RegRsi: return std::make_pair(&usr_regs_.rsi, 8);
    case X64_RegRdi: return std::make_pair(&usr_regs_.rdi, 8);
    case X64_RegRbp: return std::make_pair(&usr_regs_.rbp, 8);
    case X64_RegRsp: return std::make_pair(&usr_regs_.rsp, 8);
    case X64_RegR8:  return std::make_pair(&usr_regs_.r8 , 8);
    case X64_RegR9:  return std::make_pair(&usr_regs_.r9 , 8);
    case X64_RegR10: return std::make_pair(&usr_regs_.r10, 8);
    case X64_RegR11: return std::make_pair(&usr_regs_.r11, 8);
    case X64_RegR12: return std::make_pair(&usr_regs_.r12, 8);
    case X64_RegR13: return std::make_pair(&usr_regs_.r13, 8);
    case X64_RegR14: return std::make_pair(&usr_regs_.r14, 8);
    case X64_RegR15: return std::make_pair(&usr_regs_.r15, 8);
    case X64_RegRip: return std::make_pair(&usr_regs_.rip, 8);
    case X64_RegRflags: return std::make_pair(&usr_regs_.eflags, 8);
    case X64_SegCs:  return std::make_pair(&usr_regs_.cs, 8);
    case X64_SegSs:  return std::make_pair(&usr_regs_.ss, 8);
    case X64_SegDs:  return std::make_pair(&usr_regs_.ds, 8);
    case X64_SegEs:  return std::make_pair(&usr_regs_.es, 8);
    case X64_SegFs:  return std::make_pair(&usr_regs_.fs, 8);
    case X64_SegGs:  return std::make_pair(&usr_regs_.gs, 8);
    
    case X64_RegSt0: return std::make_pair(&usr_fpregs_.st_space[0], 16);
    case X64_RegSt1: return std::make_pair(&usr_fpregs_.st_space[4], 16);
    case X64_RegSt2: return std::make_pair(&usr_fpregs_.st_space[8], 16);
    case X64_RegSt3: return std::make_pair(&usr_fpregs_.st_space[12], 16);
    case X64_RegSt4: return std::make_pair(&usr_fpregs_.st_space[16], 16);
    case X64_RegSt5: return std::make_pair(&usr_fpregs_.st_space[20], 16);
    case X64_RegSt6: return std::make_pair(&usr_fpregs_.st_space[24], 16);
    case X64_RegSt7: return std::make_pair(&usr_fpregs_.st_space[28], 16);
    
    case X64_RegDr0: return std::make_pair(&u_debugreg_[0], 8);
    case X64_RegDr1: return std::make_pair(&u_debugreg_[1], 8);
    case X64_RegDr2: return std::make_pair(&u_debugreg_[2], 8);
    case X64_RegDr3: return std::make_pair(&u_debugreg_[3], 8);
    case X64_RegDr4: return std::make_pair(&u_debugreg_[4], 8);
    case X64_RegDr5: return std::make_pair(&u_debugreg_[5], 8);
    case X64_RegDr6: return std::make_pair(&u_debugreg_[6], 8);
    case X64_RegDr7: return std::make_pair(&u_debugreg_[7], 8);
  } throw std::out_of_range("invalid reg index");
}

} // namespace hdbg
