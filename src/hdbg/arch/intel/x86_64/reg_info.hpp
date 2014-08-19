#ifndef __HDBG_SRC_ARCH_INTEL_X86_64_REG_INFO_HPP__
#define __HDBG_SRC_ARCH_INTEL_X86_64_REG_INFO_HPP__

namespace hdbg {

const RegInfo x64_ah_info  {  "ah",  8, 8 },
              x64_al_info  {  "al",  8, 0 },
              x64_ax_info  {  "ax", 16, 0 , { &x64_ah_info  ,
                                              &x64_al_info  } },
              x64_eax_info { "eax", 32, 0 , { &x64_ax_info  } },
              x64_rax_info { "rax", 64, 0 , { &x64_eax_info } };

const RegInfo x64_bh_info  {  "bh",  8, 8 },
              x64_bl_info  {  "bl",  8, 0 },
              x64_bx_info  {  "bx", 16, 0 , { &x64_bh_info  ,
                                              &x64_bl_info  } },
              x64_ebx_info { "ebx", 32, 0 , { &x64_bx_info  } },
              x64_rbx_info { "rbx", 64, 0 , { &x64_ebx_info } };

const RegInfo x64_ch_info  {  "ch",  8, 8 },
              x64_cl_info  {  "cl",  8, 0 },
              x64_cx_info  {  "cx", 16, 0 , { &x64_ch_info  ,
                                              &x64_cl_info  } },
              x64_ecx_info { "ecx", 32, 0 , { &x64_cx_info  } },
              x64_rcx_info { "rcx", 64, 0 , { &x64_ecx_info } };

const RegInfo x64_dh_info  {  "dh",  8, 8 },
              x64_dl_info  {  "dl",  8, 0 },
              x64_dx_info  {  "dx", 16, 0 , { &x64_dh_info  ,
                                              &x64_dl_info  } },
              x64_edx_info { "edx", 32, 0 , { &x64_dx_info  } },
              x64_rdx_info { "rdx", 64, 0 , { &x64_edx_info } };

const RegInfo x64_sil_info { "sil",  8, 0 },
              x64_si_info  {  "si", 16, 0 , { &x64_sil_info } },
              x64_esi_info { "esi", 32, 0 , { &x64_si_info  } },
              x64_rsi_info { "rsi", 64, 0 , { &x64_esi_info } };

const RegInfo x64_dil_info { "dil",  8, 0 },
              x64_di_info  {  "di", 16, 0 , { &x64_dil_info } },
              x64_edi_info { "edi", 32, 0 , { &x64_di_info  } },
              x64_rdi_info { "rdi", 64, 0 , { &x64_edi_info } };

const RegInfo x64_bpl_info { "bpl",  8, 0 },
              x64_bp_info  {  "bp", 16, 0 , { &x64_bpl_info } },
              x64_ebp_info { "ebp", 32, 0 , { &x64_bp_info  } },
              x64_rbp_info { "rbp", 64, 0 , { &x64_ebp_info } };

const RegInfo x64_spl_info { "spl",  8, 0 },
              x64_sp_info  {  "sp", 16, 0 , { &x64_spl_info } },
              x64_esp_info { "esp", 32, 0 , { &x64_sp_info  } },
              x64_rsp_info { "rsp", 64, 0 , { &x64_esp_info } };

const RegInfo x64_r8b_info { "r8b",  8, 0 },
              x64_r8w_info { "r8w", 16, 0 , { &x64_r8b_info } },
              x64_r8d_info { "r8d", 32, 0 , { &x64_r8w_info } },
              x64_r8_info  {  "r8", 64, 0 , { &x64_r8d_info } };

const RegInfo x64_r9b_info { "r9b",  8, 0 },
              x64_r9w_info { "r9w", 16, 0 , { &x64_r9b_info } },
              x64_r9d_info { "r9d", 32, 0 , { &x64_r9w_info } },
              x64_r9_info  {  "r9", 64, 0 , { &x64_r9d_info } };

const RegInfo x64_r10b_info { "r10b",  8, 0 },
              x64_r10w_info { "r10w", 16, 0 , { &x64_r10b_info } },
              x64_r10d_info { "r10d", 32, 0 , { &x64_r10w_info } },
              x64_r10_info  {  "r10", 64, 0 , { &x64_r10d_info } };

const RegInfo x64_r11b_info { "r11b",  8, 0 },
              x64_r11w_info { "r11w", 16, 0 , { &x64_r11b_info } },
              x64_r11d_info { "r11d", 32, 0 , { &x64_r11w_info } },
              x64_r11_info  {  "r11", 64, 0 , { &x64_r11d_info } };

const RegInfo x64_r12b_info { "r12b",  8, 0 },
              x64_r12w_info { "r12w", 16, 0 , { &x64_r12b_info } },
              x64_r12d_info { "r12d", 32, 0 , { &x64_r12w_info } },
              x64_r12_info  {  "r12", 64, 0 , { &x64_r12d_info } };

const RegInfo x64_r13b_info { "r13b",  8, 0 },
              x64_r13w_info { "r13w", 16, 0 , { &x64_r13b_info } },
              x64_r13d_info { "r13d", 32, 0 , { &x64_r13w_info } },
              x64_r13_info  {  "r13", 64, 0 , { &x64_r13d_info } };

const RegInfo x64_r14b_info { "r14b",  8, 0 },
              x64_r14w_info { "r14w", 16, 0 , { &x64_r14b_info } },
              x64_r14d_info { "r14d", 32, 0 , { &x64_r14w_info } },
              x64_r14_info  {  "r14", 64, 0 , { &x64_r14d_info } };

const RegInfo x64_r15b_info { "r15b",  8, 0 },
              x64_r15w_info { "r15w", 16, 0 , { &x64_r15b_info } },
              x64_r15d_info { "r15d", 32, 0 , { &x64_r15w_info } },
              x64_r15_info  {  "r15", 64, 0 , { &x64_r15d_info } };

const RegInfo x64_ip_info  {  "ip", 16, 0 },
              x64_eip_info { "eip", 32, 0 , { &x64_ip_info  } },
              x64_rip_info { "rip", 64, 0 , { &x64_eip_info } };

const RegInfo x64_flg_cf   {   "CF", 1,  0 },
              x64_flg_pf   {   "PF", 1,  2 },
              x64_flg_af   {   "AF", 1,  4 },
              x64_flg_zf   {   "ZF", 1,  6 },
              x64_flg_sf   {   "SF", 1,  7 },
              x64_flg_tf   {   "TF", 1,  8 },
              x64_flg_if   {   "IF", 1,  9 },
              x64_flg_df   {   "DF", 1, 10 },
              x64_flg_of   {   "OF", 1, 11 },
              x64_flg_iopl { "IOPL", 2, 12 },
              x64_flg_nt   {   "NT", 1, 14 },
              x64_flg_rf   {   "RF", 1, 16 },
              x64_flg_vm   {   "VM", 1, 17 },
              x64_flg_ac   {   "AC", 1, 18 },
              x64_flg_vif  {  "VIF", 1, 19 },
              x64_flg_vip  {  "VIP", 1, 20 },
              x64_flg_id   {   "ID", 1, 21 };

const RegInfo x64_flags_info  {  "flags", 16, 0 , { &x64_flg_cf,  &x64_flg_pf   ,
                                                    &x64_flg_af,  &x64_flg_zf   ,
                                                    &x64_flg_sf,  &x64_flg_tf   ,
                                                    &x64_flg_if,  &x64_flg_df   ,
                                                    &x64_flg_of,  &x64_flg_iopl ,
                                                    &x64_flg_nt                 } },
              x64_eflags_info { "eflags", 32, 0 , { &x64_flags_info,
                                                    &x64_flg_rf,  &x64_flg_vm   ,
                                                    &x64_flg_ac,  &x64_flg_vif  ,
                                                    &x64_flg_vip, &x64_flg_id   } },
              x64_rflags_info { "rflags", 64, 0 , { &x64_eflags_info } };

const RegInfo x64_cs_info { "cs", 16, 0 },
              x64_ss_info { "ss", 16, 0 },
              x64_ds_info { "ds", 16, 0 },
              x64_es_info { "es", 16, 0 },
              x64_fs_info { "fs", 16, 0 },
              x64_gs_info { "gs", 16, 0 };

const RegInfo x64_st0_info { "st0", 128, 0 },
              x64_st1_info { "st1", 128, 0 },
              x64_st2_info { "st2", 128, 0 },
              x64_st3_info { "st3", 128, 0 },
              x64_st4_info { "st4", 128, 0 },
              x64_st5_info { "st5", 128, 0 },
              x64_st6_info { "st6", 128, 0 },
              x64_st7_info { "st7", 128, 0 };

const RegInfo x64_dr0_info { "dr0", 64, 0 },
              x64_dr1_info { "dr1", 64, 0 },
              x64_dr2_info { "dr2", 64, 0 },
              x64_dr3_info { "dr3", 64, 0 },
              x64_dr4_info { "dr4", 64, 0 },
              x64_dr5_info { "dr5", 64, 0 },
              x64_dr6_info { "dr6", 64, 0 },
              x64_dr7_info { "dr7", 64, 0 };

} // namespace hdbg

#endif // __HDBG_SRC_ARCH_INTEL_X86_64_REG_INFO_HPP__
