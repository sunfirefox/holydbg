#include <hdbg/dbg/thread_context.hpp>

#include <hdbg/arch_services.hpp>
#include <hdbg/arch/raw_context.hpp>
#include <hdbg/binfmt/from_image.hpp>
#include <hdbg/dbg/debug_process.hpp>
#include <hdbg/dbg/local/debug_thread.hpp>

#include "../arch/arch_internals.hpp"

namespace hdbg {

namespace {

const char * thread_arch(const DebugThread & dbg_thr)
{
  const auto& dbg_proc = dbg_thr.process();
  const auto proc_img = binfmt_from_image(dbg_proc, dbg_proc.image_base());
  return proc_img->arch();
}

} // namespace

ThreadContext::ThreadContext() = default;

ThreadContext::ThreadContext(const ThreadContext & ) {}

ThreadContext::ThreadContext(ThreadContext && ) = default;
ThreadContext::~ThreadContext() = default;

void ThreadContext::do_get_thread_context(const LocalDebugThread & dbg_thr)
{
  const auto thr_arch = thread_arch(dbg_thr);
  if(!raw_ctx_ || !raw_ctx_->valid_for(thr_arch)) {
    auto& arch_svc = get_arch_services(thr_arch);
    auto& arch_internl = arch_svc.get_internals();
    raw_ctx_ = arch_internl.make_raw_context();
  }
  raw_ctx_->obtain_from(dbg_thr);
}

void ThreadContext::do_set_thread_context(LocalDebugThread & dbg_thr) const
{
  if(raw_ctx_)
    raw_ctx_->apply_to(dbg_thr);
}

} // namespace hdbg
