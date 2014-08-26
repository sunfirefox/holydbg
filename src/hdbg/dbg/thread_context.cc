#include <hdbg/dbg/thread_context.hpp>

#include <hdbg/arch_services.hpp>
#include <hdbg/binfmt/from_image.hpp>
#include <hdbg/dbg/debug_process.hpp>
#include <hdbg/dbg/local/debug_thread.hpp>

#include "../arch/arch_internals.hpp"
#include "../arch/raw_context.hpp"

namespace hdbg {

ThreadContext::ThreadContext() = default;
ThreadContext::ThreadContext(const ThreadContext & ) = default;
ThreadContext::ThreadContext(ThreadContext && ) = default;
ThreadContext::~ThreadContext() = default;

RegValue ThreadContext::reg_value(unsigned int reg_idx) const
{
  if(!raw_ctx_)
    throw std::out_of_range("invalid reg index");
  return raw_ctx_->reg_value(reg_idx);
}

void ThreadContext::set_reg(unsigned int reg_idx, const RegValue & rv)
{
  if(!raw_ctx_)
    throw std::out_of_range("invalid reg index");
  
  if(raw_ctx_.use_count() > 1)
    raw_ctx_ = raw_ctx_->shared_clone();
  raw_ctx_->set_reg(reg_idx, rv);
}

void ThreadContext::obtain_from(const LocalDebugThread & dbg_thr)
{
  if(!raw_ctx_ || !raw_ctx_->valid_for(dbg_thr)) {
    const auto thr_arch = dbg_thr.process().image().arch();
    auto& arch_svc = get_arch_services(thr_arch);
    auto& arch_internl = arch_svc.get_internals();
    raw_ctx_ = arch_internl.make_raw_context();
  } else if(raw_ctx_.use_count() > 1) { // cow
    raw_ctx_ = raw_ctx_->shared_clone();
  }
  raw_ctx_->obtain_from(dbg_thr);
}

void ThreadContext::apply_to(LocalDebugThread & dbg_thr) const
{
  if(raw_ctx_)
    raw_ctx_->apply_to(dbg_thr);
}

} // namespace hdbg
