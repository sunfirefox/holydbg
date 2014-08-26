#ifndef __HDBG_SRC_ARCH_INTEL_X86_64_RAW_CONTEXT_HPP__
#define __HDBG_SRC_ARCH_INTEL_X86_64_RAW_CONTEXT_HPP__

#include <hdbg/arch/raw_context.hpp>

namespace hdbg {

namespace detail {

struct x64_user_regs_struct
{
  unsigned long long int r15;
  unsigned long long int r14;
  unsigned long long int r13;
  unsigned long long int r12;
  unsigned long long int rbp;
  unsigned long long int rbx;
  unsigned long long int r11;
  unsigned long long int r10;
  unsigned long long int r9;
  unsigned long long int r8;
  unsigned long long int rax;
  unsigned long long int rcx;
  unsigned long long int rdx;
  unsigned long long int rsi;
  unsigned long long int rdi;
  unsigned long long int orig_rax;
  unsigned long long int rip;
  unsigned long long int cs;
  unsigned long long int eflags;
  unsigned long long int rsp;
  unsigned long long int ss;
  unsigned long long int fs_base;
  unsigned long long int gs_base;
  unsigned long long int ds;
  unsigned long long int es;
  unsigned long long int fs;
  unsigned long long int gs;
};

struct x64_user_fpregs_struct
{
  unsigned short int cwd;
  unsigned short int swd;
  unsigned short int ftw;
  unsigned short int fop;
  unsigned long long int rip;
  unsigned long long int rdp;
  unsigned int mxcsr;
  unsigned int mxcr_mask;
  unsigned int st_space[32];  // 8*16 bytes for each FP-reg = 128 bytes
  unsigned int xmm_space[64]; // 16*16 bytes for each XMM-reg = 256 bytes
  unsigned int padding[24];
};

struct x64_user
{
  x64_user_regs_struct regs;
  int u_fpvalid;
  x64_user_fpregs_struct i387;
  unsigned long long int u_tsize;
  unsigned long long int u_dsize;
  unsigned long long int u_ssize;
  unsigned long long int start_code;
  unsigned long long int start_stack;
  long long int signal;
  int reserved;
  union {
    x64_user_regs_struct * u_ar0;
    unsigned long long int __u_ar0_word;
  };
  union {
    struct user_fpregs_struct * u_fpstate;
    unsigned long long int __u_fpstate_word;
  };
  unsigned long long int magic;
  char u_comm[32];
  unsigned long long int u_debugreg[8];
};

} // namespace detail

class X64_RawContext final
  : public RawContext
{
public:
  X64_RawContext();
  X64_RawContext(const X64_RawContext &);
  X64_RawContext(X64_RawContext &&);
  virtual ~X64_RawContext();
  
  virtual std::unique_ptr<RawContext> clone() const override;
  virtual std::shared_ptr<RawContext> shared_clone() const override;
  
  virtual bool valid_for(const char * arch) const override;
  virtual void obtain_from(const LocalDebugThread &) override;
  virtual void apply_to(LocalDebugThread &) const override;
  
  virtual std::pair<const void *, std::size_t> reg_addr(unsigned int reg_idx) const override;
  
private:
  detail::x64_user_regs_struct usr_regs_;
  detail::x64_user_fpregs_struct usr_fpregs_;
  unsigned long long int u_debugreg_[8];
};

} // namespace hdbg

#endif // __HDBG_SRC_ARCH_INTEL_X86_64_RAW_CONTEXT_HPP__
