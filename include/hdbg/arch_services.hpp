#ifndef __HDBG_ARCH_SERVICES_HPP__
#define __HDBG_ARCH_SERVICES_HPP__

#include <hdbg/config.h>
#include <hdbg/arch/reg_info.hpp>
#include <hdbg/arch/reg_value.hpp>

#include <cstddef>
#include <cstdint>
#include <vector>

namespace hdbg {

class ArchInternals;
class ThreadContext;
class CodeTracer;

class HDBG_API ArchServices
{
public:
  virtual ~ArchServices() = default;
  
  virtual const RegInfo & reg_info(unsigned int reg_idx) const  = 0;
  virtual const std::vector<RegCategoryInfo> & reg_categories() const = 0;
  
  virtual RegValue get_inst_ptr(const ThreadContext & thr_ctx) const = 0;
  virtual void set_inst_ptr(ThreadContext & thr_ctx, const RegValue & rv) const = 0;
  
  virtual RegValue get_stack_ptr(const ThreadContext & thr_ctx) const = 0;
  virtual void set_stack_ptr(ThreadContext & thr_ctx, const RegValue & rv) const = 0;
  
  virtual RegValue get_stack_base(const ThreadContext & thr_ctx) const = 0;
  virtual void set_stack_base(ThreadContext & thr_ctx, const RegValue & rv) const = 0;
  
  virtual unsigned int get_flag(const ThreadContext & thr_ctx, unsigned int flg_idx) const = 0;
  virtual void set_flag(ThreadContext & thr_ctx, unsigned int flg_idx, unsigned int value) const = 0;
  
  virtual void run_trace(CodeTracer & tracer, std::uintptr_t vaddr, const void * data,
                         std::size_t len, std::vector<std::uintptr_t> & untraced) const = 0;
  virtual void trace_resolve_untraced(CodeTracer & tracer, std::uintptr_t vaddr,
                                      const void * data, std::size_t len,
                                      std::vector<std::uintptr_t> & untraced) const = 0;
  
  virtual ArchInternals & get_internals() const = 0;
};

HDBG_API ArchServices & get_arch_services(const char * arch);

} // namespace hdbg

#endif // __HDBG_ARCH_SERVICES_HPP__
