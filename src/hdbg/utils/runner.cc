#include "runner.hpp"

#include <cassert>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <thread>
#include <utility>

namespace hdbg {

class Runner::Impl
{
public:
  enum class Command { None, ExecFn, Ret, Done };
  
  Impl();
  ~Impl();
  
  Command cmd_;
  std::function<void()> fn_;
  std::exception_ptr eptr_;
  
  std::mutex mtx_;
  std::condition_variable cv_;
  std::thread worker_;
};

Runner::Impl::Impl()
  : cmd_( Command::None )
  , worker_([this] {
      for(;;) {
        std::unique_lock<std::mutex> lock( mtx_ );
        cv_.wait(lock, [this] {
          return cmd_ == Command::ExecFn ||
                 cmd_ == Command::Ret    ;
        });
        
        switch(cmd_) {
          case Command::ExecFn:
            try {
              fn_();
              eptr_ = nullptr;
            } catch(...) {
              eptr_ = std::current_exception();
            }
            break;
          
          case Command::Ret:
            return;
          
          default:
            assert(false);
        }
        
        cmd_ = Command::Done;
        cv_.notify_one();
      }
    }) {}

Runner::Impl::~Impl()
{
  cmd_ = Command::Ret;
  cv_.notify_all();
  worker_.join();
}

Runner::Runner(): pimpl_( new Impl ) {}
Runner::Runner(Runner &&) = default;
Runner::~Runner() = default;

Runner & Runner::operator=(Runner &&) = default;

void Runner::do_run(std::function<void()> fn)
{
  std::unique_lock<std::mutex> lock( pimpl_->mtx_ );
  pimpl_->cmd_ = Impl::Command::ExecFn;
  pimpl_->fn_ = std::move(fn);
  pimpl_->cv_.notify_one();
  
  pimpl_->cv_.wait(lock, [this] {
    return pimpl_->cmd_ == Impl::Command::Done;
  });
  pimpl_->cmd_ = Impl::Command::None;
  
  if(pimpl_->eptr_)
    std::rethrow_exception(pimpl_->eptr_);
}

} // namespace hdbg
