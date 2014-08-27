#include "../../../../../arch/intel/x86_64/raw_context.hpp"
#include "../../../../../arch/intel/x86_64/reg_ids.hpp"

#include <hdbg/binary_format.hpp>
#include <hdbg/arch/endian.hpp>
#include <hdbg/dbg/debug_process.hpp>
#include <hdbg/dbg/thread_context.hpp>
#include <hdbg/dbg/local/debug_thread.hpp>

#include <sys/ptrace.h>

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <system_error>

namespace hdbg {

namespace {

RegValue read_st_reg(const unsigned int * st_ptr)
{
  RegValue rv;
  const std::size_t sizeof_reg_st = 32;
  const unsigned int reg_st_uint_entries = sizeof_reg_st / sizeof(unsigned int);
  for(int i = 0; i < reg_st_uint_entries; ++i) {
    rv += st_ptr[i];
    rv <<= std::numeric_limits<unsigned int>::digits;
  }
  return rv;
}

#if false
void write_st_reg(unsigned int * st_ptr, const RegValue & rv)
{

}
#endif

} // namespace

X64_RawContext::X64_RawContext() = default;
X64_RawContext::X64_RawContext(const X64_RawContext &) = default;
X64_RawContext::X64_RawContext(X64_RawContext && raw_ctx) = default;
X64_RawContext::~X64_RawContext() = default;

std::unique_ptr<RawContext> X64_RawContext::clone() const
{
  return std::unique_ptr<RawContext>( new X64_RawContext(*this) );
}

std::shared_ptr<RawContext> X64_RawContext::shared_clone() const
{
  return std::make_shared<X64_RawContext>(*this);
}

bool X64_RawContext::valid_for(const LocalDebugThread & dbg_thr) const
{
  const auto& dbg_proc = dbg_thr.process();
  const auto& proc_img = dbg_proc.image();
  const auto proc_arch = proc_img.arch();
  return !std::strcmp("x86_64", proc_arch);
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

RegValue X64_RawContext::reg_value(unsigned int reg_idx) const
{
  switch(reg_idx) {
    case X64_RegRax: return usr_regs_.rax;
    case X64_RegRbx: return usr_regs_.rbx;
    case X64_RegRcx: return usr_regs_.rcx;
    case X64_RegRdx: return usr_regs_.rdx;
    case X64_RegRsi: return usr_regs_.rsi;
    case X64_RegRdi: return usr_regs_.rdi;
    case X64_RegRbp: return usr_regs_.rbp;
    case X64_RegRsp: return usr_regs_.rsp;
    case X64_RegR8:  return usr_regs_.r8;
    case X64_RegR9:  return usr_regs_.r9;
    case X64_RegR10: return usr_regs_.r10;
    case X64_RegR11: return usr_regs_.r11;
    case X64_RegR12: return usr_regs_.r12;
    case X64_RegR13: return usr_regs_.r13;
    case X64_RegR14: return usr_regs_.r14;
    case X64_RegR15: return usr_regs_.r15;
    case X64_RegRip: return usr_regs_.rip;
    case X64_RegRflags: return usr_regs_.eflags;
    case X64_SegCs:  return usr_regs_.cs;
    case X64_SegSs:  return usr_regs_.ss;
    case X64_SegDs:  return usr_regs_.ds;
    case X64_SegEs:  return usr_regs_.es;
    case X64_SegFs:  return usr_regs_.fs;
    case X64_SegGs:  return usr_regs_.gs;
    
    case X64_RegSt0: return read_st_reg(&usr_fpregs_.st_space[0 * 4]);
    case X64_RegSt1: return read_st_reg(&usr_fpregs_.st_space[1 * 4]);
    case X64_RegSt2: return read_st_reg(&usr_fpregs_.st_space[2 * 4]);
    case X64_RegSt3: return read_st_reg(&usr_fpregs_.st_space[3 * 4]);
    case X64_RegSt4: return read_st_reg(&usr_fpregs_.st_space[4 * 4]);
    case X64_RegSt5: return read_st_reg(&usr_fpregs_.st_space[5 * 4]);
    case X64_RegSt6: return read_st_reg(&usr_fpregs_.st_space[6 * 4]);
    case X64_RegSt7: return read_st_reg(&usr_fpregs_.st_space[7 * 4]);
    
    case X64_RegDr0: return u_debugreg_[0];
    case X64_RegDr1: return u_debugreg_[1];
    case X64_RegDr2: return u_debugreg_[2];
    case X64_RegDr3: return u_debugreg_[3];
    case X64_RegDr4: return u_debugreg_[4];
    case X64_RegDr5: return u_debugreg_[5];
    case X64_RegDr6: return u_debugreg_[6];
    case X64_RegDr7: return u_debugreg_[7];
    
    default: throw std::out_of_range("invalid reg index");
  }
}

void X64_RawContext::set_reg(unsigned int reg_idx, const RegValue & value)
{
  switch(reg_idx) {
    case X64_RegRax: usr_regs_.rax = static_cast<std::uint64_t>( value ); break;
    case X64_RegRbx: usr_regs_.rbx = static_cast<std::uint64_t>( value ); break;
    case X64_RegRcx: usr_regs_.rcx = static_cast<std::uint64_t>( value ); break;
    case X64_RegRdx: usr_regs_.rdx = static_cast<std::uint64_t>( value ); break;
    case X64_RegRsi: usr_regs_.rsi = static_cast<std::uint64_t>( value ); break;
    case X64_RegRdi: usr_regs_.rdi = static_cast<std::uint64_t>( value ); break;
    case X64_RegRbp: usr_regs_.rbp = static_cast<std::uint64_t>( value ); break;
    case X64_RegRsp: usr_regs_.rsp = static_cast<std::uint64_t>( value ); break;
    case X64_RegR8:  usr_regs_.r8  = static_cast<std::uint64_t>( value ); break;
    case X64_RegR9:  usr_regs_.r9  = static_cast<std::uint64_t>( value ); break;
    case X64_RegR10: usr_regs_.r10 = static_cast<std::uint64_t>( value ); break;
    case X64_RegR11: usr_regs_.r11 = static_cast<std::uint64_t>( value ); break;
    case X64_RegR12: usr_regs_.r12 = static_cast<std::uint64_t>( value ); break;
    case X64_RegR13: usr_regs_.r13 = static_cast<std::uint64_t>( value ); break;
    case X64_RegR14: usr_regs_.r14 = static_cast<std::uint64_t>( value ); break;
    case X64_RegR15: usr_regs_.r15 = static_cast<std::uint64_t>( value ); break;
    case X64_RegRip: usr_regs_.rip = static_cast<std::uint64_t>( value ); break;
    case X64_RegRflags: usr_regs_.eflags = static_cast<std::uint64_t>( value ); break;
    case X64_SegCs:  usr_regs_.cs = static_cast<std::uint64_t>( value ); break;
    case X64_SegSs:  usr_regs_.ss = static_cast<std::uint64_t>( value ); break;
    case X64_SegDs:  usr_regs_.ds = static_cast<std::uint64_t>( value ); break;
    case X64_SegEs:  usr_regs_.es = static_cast<std::uint64_t>( value ); break;
    case X64_SegFs:  usr_regs_.fs = static_cast<std::uint64_t>( value ); break;
    case X64_SegGs:  usr_regs_.gs = static_cast<std::uint64_t>( value ); break;
    
#if false
    case X64_RegSt0: write_st_reg(&usr_fpregs_.st_space[0 * 4], value); break;
    case X64_RegSt1: write_st_reg(&usr_fpregs_.st_space[1 * 4], value); break;
    case X64_RegSt2: write_st_reg(&usr_fpregs_.st_space[2 * 4], value); break;
    case X64_RegSt3: write_st_reg(&usr_fpregs_.st_space[3 * 4], value); break;
    case X64_RegSt4: write_st_reg(&usr_fpregs_.st_space[4 * 4], value); break;
    case X64_RegSt5: write_st_reg(&usr_fpregs_.st_space[5 * 4], value); break;
    case X64_RegSt6: write_st_reg(&usr_fpregs_.st_space[6 * 4], value); break;
    case X64_RegSt7: write_st_reg(&usr_fpregs_.st_space[7 * 4], value); break;
#endif
    
    case X64_RegDr0: u_debugreg_[0] = static_cast<std::uint64_t>( value ); break;
    case X64_RegDr1: u_debugreg_[1] = static_cast<std::uint64_t>( value ); break;
    case X64_RegDr2: u_debugreg_[2] = static_cast<std::uint64_t>( value ); break;
    case X64_RegDr3: u_debugreg_[3] = static_cast<std::uint64_t>( value ); break;
    case X64_RegDr4: u_debugreg_[4] = static_cast<std::uint64_t>( value ); break;
    case X64_RegDr5: u_debugreg_[5] = static_cast<std::uint64_t>( value ); break;
    case X64_RegDr6: u_debugreg_[6] = static_cast<std::uint64_t>( value ); break;
    case X64_RegDr7: u_debugreg_[7] = static_cast<std::uint64_t>( value ); break;
    
    default: throw std::out_of_range("invalid reg index");
  }
}

} // namespace hdbg
