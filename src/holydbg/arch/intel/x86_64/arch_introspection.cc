#include "arch_introspection.hpp"

#include <hdbg/arch/x86_64/reg_ids.hpp>

#include "reg_info.hpp"

#include <stdexcept>

namespace holydbg {

namespace {

template <typename T, unsigned int N>
constexpr unsigned int array_size(const T (& )[N])
{
  return N;
}

} // namespace

X64_ArchIntrospection x64_introspection;

X64_ArchIntrospection::X64_ArchIntrospection() = default;
X64_ArchIntrospection::~X64_ArchIntrospection() = default;

const RegInfo & X64_ArchIntrospection::reg_info(unsigned int reg_idx) const
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

const std::vector<RegCategoryInfo> & X64_ArchIntrospection::reg_categories() const
{
  using namespace hdbg;
  
  static const std::vector<RegCategoryInfo> x64_reg_categories = {
    { "General purpose",
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

} // namespace holydbg

