#include "arch_services.hpp"

#include <hdbg/dbg/thread_context.hpp>

#include "reg_ids.hpp"
#include "reg_info.hpp"
#include "arch_internals.hpp"

#include <cstring>
#include <utility>
#include <stdexcept>

namespace hdbg {

namespace {

const std::pair<unsigned int, unsigned int> x64_flg_table[] = {
  /* CF  */ std::make_pair( 0, 1), /* PF   */ std::make_pair( 2, 1),
  /* AF  */ std::make_pair( 4, 1), /* ZF   */ std::make_pair( 6, 1),
  /* SF  */ std::make_pair( 7, 1), /* TF   */ std::make_pair( 8, 1),
  /* IF  */ std::make_pair( 9, 1), /* DF   */ std::make_pair(10, 1),
  /* OF  */ std::make_pair(11, 1), /* IOPL */ std::make_pair(12, 3),
  /* NT  */ std::make_pair(14, 1), /* RF   */ std::make_pair(16, 1),
  /* VM  */ std::make_pair(17, 1), /* AC   */ std::make_pair(18, 1),
  /* VIF */ std::make_pair(19, 1), /* VIP  */ std::make_pair(20, 1),
  /* ID  */ std::make_pair(21, 1),
};

template <typename T, unsigned int N>
constexpr unsigned int array_size(const T (& )[N])
{
  return N;
}

} // namespace

X64_ArchServices x64_services;

X64_ArchServices::X64_ArchServices() = default;
X64_ArchServices::~X64_ArchServices() = default;

const std::vector<RegCategoryInfo> & X64_ArchServices::reg_categories() const
{
  static const std::vector<RegCategoryInfo> x64_reg_categories = {
    { "General purpose registers",
       { X64_RegRax, X64_RegRbx ,
         X64_RegRcx, X64_RegRdx ,
         X64_RegRsi, X64_RegRdi ,
         X64_RegRbp, X64_RegRsp ,
         X64_RegR8 , X64_RegR9  ,
         X64_RegR10, X64_RegR11 ,
         X64_RegR12, X64_RegR13 ,
         X64_RegR14, X64_RegR15 ,
         X64_RegRip, X64_RegRflags }
    },
    { "Segment registers",
       { X64_SegCs, X64_SegSs ,
         X64_SegDs, X64_SegEs ,
         X64_SegFs, X64_SegGs }
    },
    { "Fpu registers",
       { X64_RegSt0, X64_RegSt1 ,
         X64_RegSt2, X64_RegSt3 ,
         X64_RegSt4, X64_RegSt5 ,
         X64_RegSt6, X64_RegSt7 }
    },
    { "Debug registers",
       { X64_RegDr0, X64_RegDr1 ,
         X64_RegDr2, X64_RegDr3 ,
         X64_RegDr4, X64_RegDr5 ,
         X64_RegDr6, X64_RegDr7 }
    }
  };
  return x64_reg_categories;
}

const RegInfo & X64_ArchServices::reg_info(unsigned int reg_idx) const
{
  static const RegInfo * reg_info_table[] = {
    // gen purpose
    &x64_rax_info, &x64_rbx_info, &x64_rcx_info, &x64_rdx_info,
    &x64_rsi_info, &x64_rdi_info, &x64_rbp_info, &x64_rsp_info,
    &x64_r8_info,  &x64_r9_info,  &x64_r10_info, &x64_r11_info,
    &x64_r12_info, &x64_r13_info, &x64_r14_info, &x64_r15_info,
    &x64_rip_info, &x64_rflags_info, &x64_cs_info, &x64_ss_info,
    &x64_ds_info,  &x64_es_info,  &x64_fs_info,  &x64_gs_info,
    
    // fpu regs
    &x64_st0_info, &x64_st1_info, &x64_st2_info, &x64_st3_info,
    &x64_st4_info, &x64_st5_info, &x64_st6_info, &x64_st7_info,
    
    // debug regs
    &x64_dr0_info, &x64_dr1_info, &x64_dr2_info, &x64_dr3_info,
    &x64_dr4_info, &x64_dr5_info, &x64_dr6_info, &x64_dr7_info
  };
  
  if(reg_idx >= array_size(reg_info_table))
    throw std::invalid_argument("invalid reg index");
  return *reg_info_table[ reg_idx ];
}

std::uintptr_t X64_ArchServices::get_inst_ptr(const ThreadContext & thr_ctx) const
{
  return thr_ctx.reg_value<std::uint64_t>(X64_RegRip);
}

void X64_ArchServices::set_inst_ptr(ThreadContext & thr_ctx, std::uintptr_t value) const
{
  thr_ctx.set_reg<std::uint64_t>(X64_RegRip, value);
}

std::uintptr_t X64_ArchServices::get_stack_ptr(const ThreadContext & thr_ctx) const
{
  return thr_ctx.reg_value<std::uint64_t>(X64_RegRsp);
}

void X64_ArchServices::set_stack_ptr(ThreadContext & thr_ctx, std::uintptr_t value) const
{
  thr_ctx.set_reg<std::uint64_t>(X64_RegRsp, value);
}

std::uintptr_t X64_ArchServices::get_stack_base(const ThreadContext & thr_ctx) const
{
  return thr_ctx.reg_value<std::uint64_t>(X64_RegRbp);
}

void X64_ArchServices::set_stack_base(ThreadContext & thr_ctx, std::uintptr_t value) const
{
  thr_ctx.set_reg<std::uint64_t>(X64_RegRbp, value);
}

unsigned int X64_ArchServices::get_flag(const ThreadContext & thr_ctx, unsigned int flg_idx) const
{
  if(flg_idx >= array_size(x64_flg_table))
    throw std::invalid_argument("invalid flag index");
  
  const auto& flg_e = x64_flg_table[ flg_idx ];
  const auto flg_shift = flg_e.first;
  const auto flg_mask = flg_e.second;
  const auto rflags = thr_ctx.reg_value<std::uint64_t>(X64_RegRflags);
  return (rflags >> flg_shift) & flg_mask;
}

void X64_ArchServices::set_flag(ThreadContext & thr_ctx, unsigned int flg_idx,
                                unsigned int value) const
{
  if(flg_idx >= array_size(x64_flg_table))
    throw std::invalid_argument("invalid flag index");
  
  const auto& flg_e = x64_flg_table[ flg_idx ];
  const auto flg_shift = flg_e.first;
  const auto flg_mask = flg_e.second;
  auto rflags = thr_ctx.reg_value<std::uint64_t>(X64_RegRflags);
  rflags &= ~(flg_mask << flg_shift);
  rflags |= (value & flg_mask) << flg_shift;
  thr_ctx.set_reg<std::uint64_t>(X64_RegRflags, rflags);
}

ArchInternals & X64_ArchServices::get_internals() const
{
  static X64_ArchInternals x64_internals;
  return x64_internals;
}

} // namespace hdbg
