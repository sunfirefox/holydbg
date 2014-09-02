#include "arch_services.hpp"

#include "reg_ids.hpp"
#include "reg_info.hpp"
#include "arch_internals.hpp"

#include <hdbg/arch/trace_sink.hpp>
#include <hdbg/dbg/thread_context.hpp>

#include <distorm.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <iterator>
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

RegValue X64_ArchServices::get_inst_ptr(const ThreadContext & thr_ctx) const
{
  return thr_ctx.reg_value(X64_RegRip);
}

void X64_ArchServices::set_inst_ptr(ThreadContext & thr_ctx, const RegValue & rv) const
{
  thr_ctx.set_reg(X64_RegRip, rv);
}

RegValue X64_ArchServices::get_stack_ptr(const ThreadContext & thr_ctx) const
{
  return thr_ctx.reg_value(X64_RegRsp);
}

void X64_ArchServices::set_stack_ptr(ThreadContext & thr_ctx, const RegValue & rv) const
{
  thr_ctx.set_reg(X64_RegRsp, rv);
}

RegValue X64_ArchServices::get_stack_base(const ThreadContext & thr_ctx) const
{
  return thr_ctx.reg_value(X64_RegRbp);
}

void X64_ArchServices::set_stack_base(ThreadContext & thr_ctx, const RegValue & rv) const
{
  thr_ctx.set_reg(X64_RegRbp, rv);
}

unsigned int X64_ArchServices::get_flag(const ThreadContext & thr_ctx, unsigned int flg_idx) const
{
  if(flg_idx >= array_size(x64_flg_table))
    throw std::invalid_argument("invalid flag index");
  
  const auto& flg_e = x64_flg_table[ flg_idx ];
  const auto flg_shift = flg_e.first;
  const auto flg_mask = flg_e.second;
  const auto rflags = static_cast<std::uint64_t>( thr_ctx.reg_value(X64_RegRflags) );
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
  auto rflags = static_cast<std::uint64_t>( thr_ctx.reg_value(X64_RegRflags) );
  rflags &= ~(flg_mask << flg_shift);
  rflags |= (value & flg_mask) << flg_shift;
  thr_ctx.set_reg(X64_RegRflags, rflags);
}

namespace {

struct RunTraceArgs
{
  TraceSink * tracer;
  std::uintptr_t vaddr;
  const void * data;
  std::size_t len;
  std::vector<std::uintptr_t> * code_paths;
};

void trace_code_path(const RunTraceArgs & rt_args, void * parent_block, std::uintptr_t vaddr,
                     const void * data, std::size_t len);

bool trace_fc_block(const RunTraceArgs & rt_args, void * parent_block, const _DInst & inst)
{
  assert(rt_args.tracer);
  assert(rt_args.code_paths);
  
  auto& tracer = *rt_args.tracer;
  auto& code_paths = *rt_args.code_paths;
  
  if(inst.ops[0].type != O_PC)
    return false;
  
  const std::uintptr_t vaddr_dst = INSTRUCTION_GET_TARGET(&inst);
  if(tracer.get_block(vaddr_dst))
    return true;
  
  void * const fc_block = tracer.add_block(vaddr_dst);
  tracer.link_block(parent_block, fc_block);
  code_paths.push_back(vaddr_dst);
  return true;
}

void * trace_block(const RunTraceArgs & rt_args, void * parent_block, std::uintptr_t block_beg,
                   std::uintptr_t block_end, const _DInst & inst, bool & rt_end)
{
  assert(rt_args.tracer);
  auto& tracer = *rt_args.tracer;
  
  void * const child_block = tracer.add_block(block_beg, block_end);
  tracer.link_block(parent_block, child_block);
  
  rt_end = false;
  switch(META_GET_FC(inst.meta)) {
    case FC_CALL:
      trace_fc_block(rt_args, child_block, inst);
      break;
    
    case FC_RET:
      rt_end = true;
      break;
    
    case FC_UNC_BRANCH:
      trace_fc_block(rt_args, child_block, inst);
      rt_end = true;
      break;
    
    case FC_CND_BRANCH:
      trace_fc_block(rt_args, child_block, inst);
      break;
  }
  return child_block;
}

void trace_code_path(const RunTraceArgs & rt_args, void * parent_block, std::uintptr_t vaddr,
                     const void * data, std::size_t len)
{
  assert(rt_args.tracer);
  assert(rt_args.code_paths);
  
  auto& tracer = *rt_args.tracer;
  auto& code_paths = *rt_args.code_paths;
  
  _CodeInfo ci;
  ci.codeOffset = vaddr;
  ci.code = static_cast<const std::uint8_t *>(data);
  ci.codeLen = len;
  ci.dt = Decode64Bits;
  ci.features = DF_STOP_ON_FLOW_CONTROL | DF_RETURN_FC_ONLY;
  
  while(ci.codeLen > 0) {
    unsigned int instnum;
    std::array<_DInst, 100> insts;
    ::distorm_decompose(&ci, insts.data(), insts.size(), &instnum);
    
    const std::uint8_t * code_ptr = ci.code;
    std::size_t code_len = ci.codeLen;
    std::uintptr_t code_offs = ci.codeOffset;
    
    for(int i = 0; i < instnum; ++i) {
      const _DInst & inst = insts[i];
      const std::uintptr_t block_beg = code_offs;
      const std::uintptr_t block_end = inst.addr + inst.size;
      const std::size_t block_sz = block_end - block_beg;
      
      void * child_block = tracer.get_block(inst.addr);
      if(!child_block) {
        bool trace_end;
        child_block = trace_block(rt_args, parent_block, block_beg,
                                  block_end, inst, trace_end);
        if(trace_end)
          return;
      }
      
      code_ptr += block_sz;
      code_len -= block_sz;
      code_offs += block_sz;
      parent_block = child_block;
    }
    
    const unsigned int offs = ci.nextOffset - ci.codeOffset;
    ci.code += offs;
    ci.codeLen -= offs;
    ci.codeOffset += offs;
  }
  
  code_paths.push_back(ci.codeOffset);
}

void do_run_trace(const RunTraceArgs & rt_args)
{
  assert(rt_args.tracer);
  assert(rt_args.data);
  assert(rt_args.code_paths);
  
  auto& tracer = *rt_args.tracer;
  auto& code_paths = *rt_args.code_paths;
  const auto bytes = static_cast<const std::uint8_t *>(rt_args.data);
  
  const auto vaddr = rt_args.vaddr;
  const auto vaddr_end = vaddr + rt_args.len;
  const auto addr_in_block = [vaddr, vaddr_end](std::uintptr_t addr) {
    return vaddr <= addr && addr < vaddr_end;
  };
  
  std::vector<std::uintptr_t> traceable;
  for(;;) {
    traceable.reserve(code_paths.size());
    std::copy_if(code_paths.begin(), code_paths.end(), std::back_inserter(traceable), addr_in_block);
    code_paths.erase(std::remove_if(code_paths.begin(), code_paths.end(), addr_in_block), code_paths.end());
    
    if(traceable.empty())
      break;
    
    for(auto tr_addr : traceable) {
      const auto tr_size = vaddr_end - tr_addr;
      const auto tr_offs = tr_addr - vaddr;
      const auto tr_data = bytes + tr_offs;
      trace_code_path(rt_args, tracer.root_block(), tr_addr, tr_data, tr_size);
    }
    traceable.clear();
  }
}

} // namespace

void X64_ArchServices::run_trace(TraceSink & tracer, std::uintptr_t vaddr, const void * data,
                                 std::size_t len, std::vector<std::uintptr_t> & code_paths) const
{
  if(!data)
    throw std::invalid_argument("null data ptr");
  
  const RunTraceArgs rt_args { &tracer, vaddr, data, len, &code_paths };
  do_run_trace(rt_args);
}

ArchInternals & X64_ArchServices::get_internals() const
{
  static X64_ArchInternals x64_internals;
  return x64_internals;
}

} // namespace hdbg
