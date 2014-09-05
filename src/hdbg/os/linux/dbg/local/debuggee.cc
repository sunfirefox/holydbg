#include <hdbg/dbg/local/debuggee.hpp>

#include "pt_runner.hpp"
#include "../../../../dbg/breakpoint_manager.hpp"

#include <hdbg/dbg/local/debug_process.hpp>
#include <hdbg/dbg/local/debug_thread.hpp>
#include <hdbg/enum/enum_threads.hpp>

#include <signal.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/wait.h>

#include <algorithm>
#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <utility>
#include <sstream>
#include <string>
#include <system_error>

namespace hdbg {

struct LocalDebuggee::Impl
{
  struct DbgThreadData
  {
    LocalDebugThread dbg_thr;
    unsigned long sig_deliv;
    bool stopped;
  };
  
  typedef typename std::unordered_map<thread_id, DbgThreadData>::value_type DbgThreadEntry;
  
  struct RawDebugEvent
  {
    DbgThreadEntry * thr_entry;
    int thr_status;
  };
  
  Impl(process_id pid);
  
  LocalDebugThread & add_new_thread(thread_id tid);
  void stop_all_threads();
  
  void wait_event(RawDebugEvent & evt);
  void wait_event(pid_t wpid, RawDebugEvent & evt);
  void dispatch_event(LocalDebuggee & self, const RawDebugEvent & evt);
  
  std::unique_ptr<LocalDebuggee> do_attach_child(const LocalDebuggee & self,
                                                 process_id pid, unsigned int flags);
  
  BreakpointManager bp_mgr_;
  DebugEventEmitter evt_emitter_;
  LocalDebugProcess process_;
  std::unordered_map<thread_id, DbgThreadData> threads_;
  
  process_id new_child_;
  bool child_detach_;
  bool evt_ign_;
  bool attached_;
  
private:
  void on_process_created(LocalDebuggee & self, DbgThreadEntry & thr_e, process_id new_pid);
  void on_process_exited(LocalDebuggee & self, int exit_code);
  void on_process_killed(LocalDebuggee & self, int term_sig);
  void on_thread_created(LocalDebuggee & self, DbgThreadEntry & thr_e, thread_id new_tid);
  void on_thread_exited(LocalDebuggee & self, DbgThreadEntry & thr_e, int exit_code);
  void on_sig_trap(LocalDebuggee & self, DbgThreadEntry & thr_e, const siginfo_t & si);
  void on_signal(LocalDebuggee & self, DbgThreadEntry & thr_e, const siginfo_t & si);
  void on_unknown_event(LocalDebuggee & self, const RawDebugEvent & raw_evt);
  
  bool handle_bp_event(LocalDebuggee & self, LocalDebugThread & which, const DebugEvent & evt);
  
  template <typename T> void notify_event(LocalDebuggee & self, T && evt);
};

LocalDebuggee::Impl::Impl(process_id pid)
  : process_( pid, DebugProcess::OpenFlags::AllAccess )
  , attached_( true ) {}

LocalDebugThread & LocalDebuggee::Impl::add_new_thread(thread_id tid)
{
  hdbg::LocalDebugThread dbg_thr (process_, tid, DebugThread::OpenFlags::AllAccess);
  DbgThreadData thr_data { std::move(dbg_thr), 0, true };
  const auto ins_p = threads_.emplace(tid, std::move(thr_data));
  return ins_p.first->second.dbg_thr;
}

namespace {

void tgkill(pid_t tgid, pid_t tid, int sig)
{
  if(::syscall(SYS_tgkill, tgid, tid, sig) == -1)
    throw std::system_error(errno, std::system_category());
}

} // namespace

void LocalDebuggee::Impl::stop_all_threads()
{
  for(auto& thr_e : threads_) {
    if(!thr_e.second.stopped)
      tgkill(process_.id(), thr_e.first, SIGSTOP);
  }
}

void LocalDebuggee::Impl::wait_event(RawDebugEvent & evt)
{
  for(;;) {
    for(auto& thr_e : threads_) {
      int status;
      const pid_t wtid = ::waitpid(thr_e.first, &status, __WALL | WNOHANG);
      if(wtid == -1)
        throw std::system_error(errno, std::system_category());
      
      if(wtid != 0) {
        thr_e.second.stopped = WIFSTOPPED(status);
        evt.thr_entry = &thr_e;
        evt.thr_status = status;
        return;
      }
    }
  }
}

void LocalDebuggee::Impl::wait_event(pid_t wpid, RawDebugEvent & evt)
{
  int status;
  const pid_t wtid = ::waitpid(wpid, &status, __WALL);
  if(wtid == -1)
    throw std::system_error(errno, std::system_category());
  
  const auto thr_itr = threads_.find(wtid);
  thr_itr->second.stopped = WIFSTOPPED(status);
  evt.thr_entry = &*thr_itr;
  evt.thr_status = status;
}

namespace {

unsigned long ptrace_geteventmsg(pid_t pid)
{
  unsigned long evt_msg;
  if(::ptrace(PTRACE_GETEVENTMSG, pid, nullptr, &evt_msg) == -1)
    throw std::system_error(errno, std::system_category());
  return evt_msg;
}

void ptrace_getsiginfo(pid_t pid, siginfo_t & si)
{
  if(::ptrace(PTRACE_GETSIGINFO, pid, nullptr, &si) == -1)
    throw std::system_error(errno, std::system_category());
}

} // namespace

void LocalDebuggee::Impl::dispatch_event(LocalDebuggee & self, const RawDebugEvent & evt)
{
  const pid_t wtid = evt.thr_entry->first;
  const int status = evt.thr_status;
  
  evt_ign_ = false;
  new_child_ = 0;
  if(WIFSTOPPED(status)) {
    const int wstopsig = WSTOPSIG(status);
    auto& thr_e = *evt.thr_entry;
    stop_all_threads();
    if(wstopsig == SIGTRAP) {
      switch(status >> 8) {
        case SIGTRAP | (PTRACE_EVENT_FORK << 8): {
          const pid_t new_pid = pt_runner.run([wtid] {
            return ptrace_geteventmsg(wtid);
          });
          on_process_created(self, thr_e, new_pid);
        } break;
        
        case SIGTRAP | (PTRACE_EVENT_VFORK << 8): {
          const pid_t new_pid = pt_runner.run([wtid] {
            return ptrace_geteventmsg(wtid);
          });
          on_process_created(self, thr_e, new_pid);
        } break;
        
        case SIGTRAP | (PTRACE_EVENT_CLONE << 8): {
          const pid_t new_thr_id = pt_runner.run([wtid] {
            return ptrace_geteventmsg(wtid);
          });
          on_thread_created(self, thr_e, new_thr_id);
        } break;
        
        case SIGTRAP | (PTRACE_EVENT_VFORK_DONE << 8):
          break;
        
        case SIGTRAP | (PTRACE_EVENT_EXIT << 8): {
          const int exit_code = pt_runner.run([wtid] {
            return ptrace_geteventmsg(wtid);
          });
          on_thread_exited(self, thr_e, exit_code);
        } break;
        
        default: {
          siginfo_t si;
          pt_runner.run([wtid, &si] {
            ptrace_getsiginfo(wtid, si);
          });
          on_sig_trap(self, thr_e, si);
          break;
        }
      }
    } else {
      siginfo_t si;
      pt_runner.run([wtid, &si] {
        ptrace_getsiginfo(wtid, si);
      });
      on_signal(self, thr_e, si);
    }
  } else if(WIFEXITED(status)) {
    on_process_exited(self, WEXITSTATUS(status));
  } else if(WIFSIGNALED(status)) {
    on_process_killed(self, WTERMSIG(status));
  } else {
    on_unknown_event(self, evt);
  }
}

void LocalDebuggee::Impl::on_process_created(LocalDebuggee & self, DbgThreadEntry & thr_e,
                                             process_id new_pid)
{
  new_child_ = new_pid;
  child_detach_ = true;
  
  auto& thr_which = thr_e.second.dbg_thr;
  notify_event(self, ProcessCreatedEvent{ &self, &thr_which, new_pid });
  
  if(child_detach_) {
    pt_runner.run([new_pid] {
      if(::ptrace(PTRACE_DETACH, new_pid, nullptr, nullptr) == -1)
        throw std::system_error(errno, std::system_category());
    });
  }
}

void LocalDebuggee::Impl::on_process_exited(LocalDebuggee & self, int exit_code)
{
  notify_event(self, ProcessExitedEvent{ &self, exit_code });
  attached_ = false;
}

void LocalDebuggee::Impl::on_process_killed(LocalDebuggee & self, int sig)
{
  notify_event(self, ProcessKilledEvent{ &self, sig });
  attached_ = false;
}

void LocalDebuggee::Impl::on_thread_created(LocalDebuggee & self, DbgThreadEntry & thr_e,
                                            thread_id new_tid)
{
  int status;
  if(::waitpid(new_tid, &status, __WALL) == -1)
    throw std::system_error(errno, std::system_category());
  auto& new_thr = add_new_thread(new_tid);
  
  auto& thr_which = thr_e.second.dbg_thr;
  notify_event(self, ThreadCreatedEvent{ &self, &thr_which ,&new_thr });
}

void LocalDebuggee::Impl::on_thread_exited(LocalDebuggee & self, DbgThreadEntry & thr_e,
                                           int exit_code)
{
  auto& thr_which = thr_e.second.dbg_thr;
  notify_event(self, ThreadExitedEvent{ &self, &thr_which, exit_code });
  
  const auto tid = thr_e.first;
  if(tid != process_.id())
    threads_.erase(tid);
}

void LocalDebuggee::Impl::on_sig_trap(LocalDebuggee & self, DbgThreadEntry & thr_e,
                                      const siginfo_t & si)
{
  auto& thr_which = thr_e.second.dbg_thr;
  switch(si.si_code) {
    case TRAP_BRKPT: {
      BreakpointHitEvent evt { &self, &thr_which };
      if(!handle_bp_event(self, thr_which, evt))
        return;
      notify_event(self, evt);
    } break;
    
    case TRAP_TRACE: {
      SinglestepEvent evt { &self, &thr_which };
      if(handle_bp_event(self, thr_which, evt))
        return;
      notify_event(self, evt);
    } break;
    
    default: {
      UnknownEvent evt { &self };
      if(handle_bp_event(self, thr_which, evt))
        return;
      notify_event(self, evt);
    }
  }
  thr_e.second.sig_deliv = evt_ign_ ? 0 : SIGTRAP;
}

void LocalDebuggee::Impl::on_signal(LocalDebuggee & self, DbgThreadEntry & thr_e,
                                    const siginfo_t & si)
{
  auto& thr_which = thr_e.second.dbg_thr;
  switch(si.si_signo) {
    case SIGILL: {
      IllegalInstructionEvent evt { &self, &thr_which };
      if(handle_bp_event(self, thr_which, evt))
          return;
      notify_event(self, evt);
    } break;
    
    case SIGFPE: {
      FloatingPointExceptionEvent evt { &self, &thr_which };
      if(handle_bp_event(self, thr_which, evt))
        return;
      notify_event(self, evt);
    } break;
    
    case SIGSEGV: {
      AccessViolationEvent evt { &self, &thr_which };
      if(handle_bp_event(self, thr_which, evt))
        return;
      notify_event(self, evt);
    } break;
    
    default: {
      UnknownEvent evt { &self };
      if(handle_bp_event(self, thr_which, evt))
        return;
      notify_event(self, evt);
    } break;
  }
  thr_e.second.sig_deliv = evt_ign_ ? 0 : si.si_signo;
}

void LocalDebuggee::Impl::on_unknown_event(LocalDebuggee & self,
                                           const RawDebugEvent & raw_evt)
{
  notify_event(self, UnknownEvent{ &self });
}

bool LocalDebuggee::Impl::handle_bp_event(LocalDebuggee & self, LocalDebugThread & which,
                                          const DebugEvent & dbg_evt)
{
  if(!bp_mgr_.dispatch_bp_hit(self, which, dbg_evt))
    return false;
  
  self.singlestep(which);
  bp_mgr_.restore_bps();
  return true;
}

template <typename T>
void LocalDebuggee::Impl::notify_event(LocalDebuggee & self, T && evt)
{
  evt_emitter_.emit_event(std::forward<T>(evt));
}

std::unique_ptr<LocalDebuggee>
  LocalDebuggee::Impl::do_attach_child(const LocalDebuggee & self, process_id pid,
                                       unsigned int flags)
{
  if(pid != new_child_)
    throw std::invalid_argument("invalid child pid");
  
  std::unique_ptr<LocalDebuggee> dbg_child ( new LocalDebuggee(self, pid) );

  child_detach_ = false;
  new_child_ = 0;
  return dbg_child;
}

namespace {

unsigned long translate_debug_flags(unsigned int dbg_flags)
{
  return PTRACE_O_TRACECLONE | PTRACE_O_TRACEFORK      |
         PTRACE_O_TRACEVFORK | PTRACE_O_TRACEVFORKDONE |
         PTRACE_O_TRACEEXIT  |
         ((dbg_flags & DebugFlags::KillOnExit) ? PTRACE_O_EXITKILL : 0);
}

std::string make_env_var(const std::string & var, const std::string & value)
{
  std::ostringstream oss;
  oss << var << '=' << value;
  return oss.str();
}

void ptrace_child_stub [[noreturn]] (const char * cwd, char ** argv, char ** envp)
{
  if(::chdir(cwd) == -1)
    std::exit(errno);
  
  if(::ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) == -1)
    std::exit(errno);
  
  if(::raise(SIGSTOP) != 0)
    std::exit(errno);
  
  ::execve(argv[0], argv, envp);
    std::exit(errno);
}

} // namespace

std::unique_ptr<LocalDebuggee>
  LocalDebuggee::exec(const DbgExecParams & params, unsigned int flags)
{
  std::vector<std::string> child_args { params.file };
  std::vector<char *> child_args_p { &child_args.back()[0] };
  if(params.flags & DbgExecParams::Flags::HasArgs) {
    for(auto arg : params.args) {
      child_args_p.push_back(&arg[0]);
      child_args.push_back( std::move(arg) );
    }
  }
  child_args_p.push_back(nullptr);
  char ** const child_argv = child_args_p.data();
  
  char ** child_envp = nullptr;
  std::vector<std::string> child_env_vars;
  std::vector<char *> child_env_var_p;
  if(params.flags & DbgExecParams::Flags::HasEnv) {
    for(const auto& env_p: params.env) {
      auto env_var = make_env_var(env_p.first, env_p.second);
      child_env_var_p.push_back(&env_var[0]);
      child_env_vars.emplace_back( std::move(env_var) );
    }
    child_env_var_p.push_back(nullptr);
    child_envp = child_env_var_p.data();
  } else {
    child_envp = ::environ;
  }
  
  char child_cwd[PATH_MAX+1];
  if(params.flags & DbgExecParams::Flags::HasCwd) {
    if(params.cwd.length() > PATH_MAX)
      throw std::length_error("invalid path length (exceeded MAX_PATH)");
    std::strcpy(child_cwd, params.cwd.c_str());
  } else {
    ::getcwd(child_cwd, sizeof(child_cwd));
  }
  
  const pid_t child_pid = pt_runner.run([&child_cwd, &child_argv, &child_envp] {
    const pid_t pid = ::fork();
    if(pid == -1)
      throw std::system_error(errno, std::system_category());
    
    if(pid == 0)
      ptrace_child_stub(child_cwd, child_argv, child_envp);
    return pid;
  });
  
  int status;
  if(::waitpid(child_pid, &status, WUNTRACED) == -1) // sigstop
    throw std::system_error(errno, std::system_category());
  if(WIFEXITED(status))
    throw std::system_error(WEXITSTATUS(status), std::system_category());
  
  pt_runner.run([child_pid, flags] {
    const unsigned long trace_opts = translate_debug_flags(flags);
    if(::ptrace(PTRACE_SETOPTIONS, child_pid, nullptr, trace_opts) == -1)
      throw std::system_error(errno, std::system_category());
    if(::ptrace(PTRACE_CONT, child_pid, nullptr, nullptr) == -1)
      throw std::system_error(errno, std::system_category());
  });

  if(::waitpid(child_pid, &status, 0) == -1) // sigtrap
    throw std::system_error(errno, std::system_category());
  if(WIFEXITED(status))
    throw std::system_error(WEXITSTATUS(status), std::system_category());
  
  std::unique_ptr<LocalDebuggee> debuggee ( new LocalDebuggee(child_pid) );
  debuggee->pimpl_->add_new_thread(child_pid);
  return debuggee;
}

namespace {

void ptrace_attach_task(process_id lwp, unsigned int trace_opts)
{
  if(::ptrace(PTRACE_ATTACH, lwp, nullptr, nullptr) == -1)
    throw std::system_error(errno, std::system_category());
  
  int status;
  if(::waitpid(lwp, &status, __WALL) == -1)
    throw std::system_error(errno, std::system_category());
  
  if(::ptrace(PTRACE_SETOPTIONS, lwp, nullptr, trace_opts) == -1)
    throw std::system_error(errno, std::system_category());
}

} // namespace

std::unique_ptr<LocalDebuggee> LocalDebuggee::attach(process_id pid, unsigned int flags)
{
  if(::kill(pid, SIGSTOP) == -1)
    throw std::system_error(errno, std::system_category());
  
  unsigned long trace_opts;
  pt_runner.run([&trace_opts, flags, pid] {
    trace_opts = translate_debug_flags(flags);
    ptrace_attach_task(pid, trace_opts);
  });
  
  std::unique_ptr<LocalDebuggee> debuggee ( new LocalDebuggee(pid) );
  debuggee->pimpl_->add_new_thread(pid);
  
  pt_runner.run([pid, trace_opts, &debuggee] {
    for(const auto& thr_e : enum_threads(pid)) {
      if(thr_e.tid != pid) {
        ptrace_attach_task(thr_e.tid, trace_opts);
        debuggee->pimpl_->add_new_thread(thr_e.tid);
      }
    }
  });
  return debuggee;
}

LocalDebuggee::LocalDebuggee(process_id pid)
  : pimpl_( new Impl(pid) ) {}

LocalDebuggee::LocalDebuggee(const LocalDebuggee & parent, process_id pid)
  : LocalDebuggee( pid )
{
  pimpl_->evt_emitter_ = parent.pimpl_->evt_emitter_;
}

LocalDebuggee::LocalDebuggee(LocalDebuggee &&) = default;

LocalDebuggee::~LocalDebuggee()
{
  if(pimpl_->attached_)
    std::terminate(); // detach or kill? can't decide
}

LocalDebugProcess & LocalDebuggee::process()
{
  assert( attached() );
  return pimpl_->process_;
}

const LocalDebugProcess & LocalDebuggee::process() const
{
  assert( attached() );
  return pimpl_->process_;
}

LocalDebugThread & LocalDebuggee::get_thread(thread_id tid)
{
  assert( attached() );
  return pimpl_->threads_.at(tid).dbg_thr;
}

const LocalDebugThread & LocalDebuggee::get_thread(thread_id tid) const
{
  assert( attached() );
  return pimpl_->threads_.at(tid).dbg_thr;
}

bool LocalDebuggee::attached() const
{
  return pimpl_->attached_;
}

namespace {

void ptrace_singlestep(pid_t pid, int sig)
{
  if(::ptrace(PTRACE_SINGLESTEP, pid, nullptr, sig) == -1)
    throw std::system_error(errno, std::system_category());
}

} // namespace

void LocalDebuggee::singlestep(DebugThread & run_thr)
{
  assert( attached() );
  
  const auto thr_id = run_thr.id();
  auto& thr_e = pimpl_->threads_.at(thr_id);
  if(!thr_e.stopped)
    throw std::runtime_error("thread not stopped");
  
  pt_runner.run([thr_id, &thr_e] {
    ptrace_singlestep(thr_id, thr_e.sig_deliv);
    thr_e.stopped = false;
    thr_e.sig_deliv = 0;
  });
  
  Impl::RawDebugEvent evt;
  pimpl_->wait_event(thr_id, evt);
  pimpl_->dispatch_event(*this, evt);
}

namespace {

void ptrace_cont(pid_t task_id, unsigned long sig)
{
  if(::ptrace(PTRACE_CONT, task_id, nullptr, sig) == -1)
    throw std::system_error(errno, std::system_category());
}

} // namespace

void LocalDebuggee::run()
{
  assert( attached() );
  
  while(pimpl_->attached_) {
    pt_runner.run([this] {
      for(auto& thr_e : pimpl_->threads_) {
        if(thr_e.second.stopped) {
          ptrace_cont(thr_e.first, thr_e.second.sig_deliv);
          thr_e.second.stopped = false;
          thr_e.second.sig_deliv = 0;
        }
      }
    });
    
    Impl::RawDebugEvent evt;
    pimpl_->wait_event(evt);
    pimpl_->dispatch_event(*this, evt);
  }
}

void LocalDebuggee::detach()
{
  assert( attached() );
  
  pt_runner.run([this] {
    for(auto& thr_e : pimpl_->threads_) {
      if(::ptrace(PTRACE_DETACH, thr_e.first, nullptr, nullptr) == -1)
        throw std::system_error(errno, std::system_category());
    }
  });
  
  pimpl_->attached_ = false;
}

void LocalDebuggee::kill()
{
  assert( attached() );
  pimpl_->process_.kill(true);
  pimpl_->attached_ = false;
}

void LocalDebuggee::add_listener(std::shared_ptr<DebugEventListener> sp_listener)
{
  pimpl_->evt_emitter_.add_listener(std::move(sp_listener));
}

void LocalDebuggee::remove_listener(const std::shared_ptr<DebugEventListener> & sp_listener)
{
  pimpl_->evt_emitter_.remove_listener(sp_listener);
}

void LocalDebuggee::discard_event()
{
  assert( attached() );
  pimpl_->evt_ign_ = true;
}

breakpoint_id LocalDebuggee::set_bp(Breakpoint * bp, BpHandlerFn fn)
{
  assert( attached() );
  
  if(!bp)
    throw std::invalid_argument("null bp ptr");
  return pimpl_->bp_mgr_.set_bp(*this, bp, fn);
}

void LocalDebuggee::remove_bp(breakpoint_id bp_id)
{
  assert( attached() );
  pimpl_->bp_mgr_.remove_bp(bp_id);
}

void LocalDebuggee::remove_all_bps()
{
  assert( attached() );
  pimpl_->bp_mgr_.remove_all_bps(this);
}

std::unique_ptr<Debuggee> LocalDebuggee::attach_child(process_id pid, unsigned int flags) const
{
  assert( attached() );
  return pimpl_->do_attach_child(*this, pid, flags);
}

} // namespace hdbg
