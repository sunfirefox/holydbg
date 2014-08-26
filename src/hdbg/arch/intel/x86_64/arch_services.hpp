#ifndef __HDBG_SRC_ARCH_INTEL_X86_64_ARCH_SERVICES_HPP__
#define __HDBG_SRC_ARCH_INTEL_X86_64_ARCH_SERVICES_HPP__

#include <hdbg/arch_services.hpp>

namespace hdbg {

class X64_ArchServices final
  : public ArchServices
{
public:
  X64_ArchServices();
  virtual ~X64_ArchServices();
  
  virtual const RegInfo & reg_info(unsigned int reg_idx) const override;
  virtual const std::vector<RegCategoryInfo> & reg_categories() const override;
  
  virtual RegValue get_inst_ptr(const ThreadContext & thr_ctx) const override;
  virtual void set_inst_ptr(ThreadContext & thr_ctx, const RegValue & rv) const override;
  
  virtual RegValue get_stack_ptr(const ThreadContext & thr_ctx) const override;
  virtual void set_stack_ptr(ThreadContext & thr_ctx, const RegValue & rv) const override;
  
  virtual RegValue get_stack_base(const ThreadContext & thr_ctx) const override;
  virtual void set_stack_base(ThreadContext & thr_ctx, const RegValue & rv) const override;
  
  virtual unsigned int get_flag(const ThreadContext & thr_ctx, unsigned int flg_idx) const override;
  virtual void set_flag(ThreadContext & thr_ctx, unsigned int flg_idx, unsigned int value) const override;
  
  virtual ArchInternals & get_internals() const override;
};

extern X64_ArchServices x64_services;

} // namespace hdbg

#endif // __HDBG_SRC_ARCH_INTEL_X86_64_ARCH_SERVICES_HPP__
