#include <hdbg/dbg/local/debuggee.hpp>

#include <hdbg/dbg/local/debug_process.hpp>
#include <hdbg/dbg/local/debug_thread.hpp>
#include <hdbg/enum/enum_threads.hpp>

#include "../../process_kill_guard.hpp"
#include "../../process_stop_guard.hpp"

#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstdlib>
#include <utility>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

namespace hdbg {

namespace {

unsigned long translate_debug_flags(unsigned int dbg_flags)
{
  return PTRACE_O_TRACECLONE | PTRACE_O_TRACEFORK |
         PTRACE_O_TRACEVFORK | PTRACE_O_TRACEEXIT |
         ((dbg_flags & DebugFlags::KillOnExit) ? PTRACE_O_EXITKILL : 0);
}

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

void ptrace_singlestep(pid_t pid, int sig)
{
  if(::ptrace(PTRACE_SINGLESTEP, pid, nullptr, sig) == -1)
    throw std::system_error(errno, std::system_category());
}

void tgkill(pid_t tgid, pid_t tid, int sig)
{
  if(syscall(SYS_tgkill, tgid, tid, sig) == -1)
    throw std::system_error(errno, std::system_category());
}

std::string make_env_var(const std::string & var, const std::string & value)
{
  std::ostringstream oss;
  oss << var << '=' << value;
  return oss.str();
}

} // namespace

struct LocalDebuggee::Impl
{
  struct DebugEventInternal
  {
    pid_t wtid;
    int status;
  };
  
  Impl(Debuggee & self, LocalDebugProcess && proc, int dbg_flags);
  
  LocalDebugThread & attach_new_thread(thread_id);
  LocalDebugThread & add_traced_thread(thread_id);
  
  LocalDebugThread & get_thread(thread_id);
  
  void wait_event(DebugEventInternal * dbg_evt = nullptr);
  void wait_event(pid_t wpid, DebugEventInternal * dbg_evt = nullptr);
  void dispatch_event(LocalDebuggee & self, const DebugEventInternal & dbg_evt);
  
  LocalDebugProcess process_;
  std::vector<LocalDebugThread> threads_;
  bool attached_;
  unsigned long sig_deliv_;
  bool evt_ign_;
  
private:
  typedef typename std::vector<LocalDebugThread>::iterator thread_iterator;
  thread_iterator find_thread(thread_id);
  
  void on_process_created(LocalDebuggee & self, thread_id which, process_id new_pid);
  void on_process_exited(LocalDebuggee & self, int exit_code);
  void on_process_killed(LocalDebuggee & self, int term_sig);
  void on_thread_created(LocalDebuggee & self, thread_id which, thread_id new_tid);
  void on_thread_exited(LocalDebuggee & self, thread_id which, int exit_code);
  void on_sig_trap(LocalDebuggee & self, thread_id which, const siginfo_t & si);
  void on_signal(LocalDebuggee & self, thread_id which, const siginfo_t & si);
  
  void notify_process_created(LocalDebuggee & self, LocalDebugThread & which, process_id new_pid);
  void notify_process_exited(LocalDebuggee & self, int exit_code);
  void notify_process_killed(LocalDebuggee & self, int sig);
  void notify_thread_created(LocalDebuggee & self, LocalDebugThread & which, LocalDebugThread & thr_new);
  void notify_thread_exited(LocalDebuggee & self, LocalDebugThread & which, int exit_code);
  void notify_unhandled_bp(LocalDebuggee & self, LocalDebugThread & which);
  void notify_singlestep(LocalDebuggee & self, LocalDebugThread & which);
  void notify_access_violation(LocalDebuggee & self, LocalDebugThread & which);
  
  const unsigned long trace_opts_;
};

LocalDebuggee::Impl::Impl(Debuggee & self, LocalDebugProcess && proc, int dbg_flags)
  : process_( std::move(proc) )
  , trace_opts_( translate_debug_flags(dbg_flags) )
  , attached_( true )
  , sig_deliv_( 0 )
  , evt_ign_( false )
{
  for(const auto& thr_e : enum_threads( process_.id() )) {
    if(thr_e.tid == process_.id())
      add_traced_thread(thr_e.tid); // main thread already traced
    else
      attach_new_thread(thr_e.tid);
    
    if(::ptrace(PTRACE_SETOPTIONS, thr_e.tid, nullptr, trace_opts_) == -1)
      throw std::system_error(errno, std::system_category());
  }
}

LocalDebugThread & LocalDebuggee::Impl::add_traced_thread(thread_id tid)
{
  hdbg::LocalDebugThread dbg_thr(process_, tid, DebugThread::OpenFlags::AllAccess);
  threads_.emplace_back( std::move(dbg_thr) );
  return threads_.back();
}

LocalDebugThread & LocalDebuggee::Impl::attach_new_thread(thread_id tid)
{
  if(::ptrace(PTRACE_ATTACH, tid, nullptr, nullptr) == -1)
    throw std::system_error(errno, std::system_category());
  
  if(::waitpid(tid, nullptr, __WALL) == -1)
    throw std::system_error(errno, std::system_category());
  
  return add_traced_thread(tid);
}

LocalDebugThread & LocalDebuggee::Impl::get_thread(thread_id tid)
{
  const auto thr_itr = find_thread(tid);
  if(thr_itr == threads_.end())
    throw std::invalid_argument("unknown thread");
  return *thr_itr;
}

void LocalDebuggee::Impl::wait_event(DebugEventInternal * evt)
{
  for(;;) {
    for(const auto& dbg_thr : threads_) {
      int status;
      const pid_t wtid = ::waitpid(dbg_thr.id(), &status, __WALL | WNOHANG);
      if(wtid == -1)
        throw std::system_error(errno, std::system_category());
      
      if(wtid != 0) {
        if(evt) {
          evt->wtid = wtid;
          evt->status = status;
        }
        return;
      }
    }
  }
}

void LocalDebuggee::Impl::wait_event(pid_t wpid, DebugEventInternal * evt)
{
  int status;
  const pid_t wtid = ::waitpid(wpid, &status, __WALL);
  if(wtid == -1)
    throw std::system_error(errno, std::system_category());
  
  if(evt) {
    evt->wtid = wtid;
    evt->status = status;
  }
}

void LocalDebuggee::Impl::dispatch_event(LocalDebuggee & self,
                                         const DebugEventInternal & evt)
{
  const pid_t wtid = evt.wtid;
  const int status = evt.status;
  
  evt_ign_ = false;
  sig_deliv_ = 0;
  if(WIFSTOPPED(status)) {
    std::cerr << "-> stopped (sig = " << WSTOPSIG(status) << ")" << std::endl;
    const int wstopsig = WSTOPSIG(status);
    if(wstopsig == SIGTRAP) {
      std::cerr << "[*] trapped" << std::endl;
      switch(status >> 8) {
        case SIGTRAP | (PTRACE_EVENT_CLONE << 8): {
          std::cerr << "[*] event_clone" << std::endl;
          const pid_t new_thr_id = ptrace_geteventmsg(wtid);
          on_thread_created(self, wtid, new_thr_id);
        } break;
        
        case SIGTRAP | (PTRACE_EVENT_EXIT  << 8): {
          const int exit_code = ptrace_geteventmsg(wtid);
          std::cerr << "[*] event_exit (tid = " << wtid 
                    << ", code = " << exit_code << ")" << std::endl;
          on_thread_exited(self, wtid, exit_code);
        } break;
        
        case SIGTRAP | (PTRACE_EVENT_FORK  << 8):
        case SIGTRAP | (PTRACE_EVENT_VFORK << 8): {
          std::cerr << "[*] event_xfork" << std::endl;
          const pid_t new_pid = ptrace_geteventmsg(wtid);
          on_process_created(self, wtid, new_pid);
        } break;
        
        default: {
          siginfo_t si;
          ptrace_getsiginfo(wtid, si);
          on_sig_trap(self, wtid, si);
          break;
        }
      }
    } else {
      std::cerr << "[*] sig != SIGTRAP" << std::endl;
      siginfo_t si;
      ptrace_getsiginfo(wtid, si);
      on_signal(self, wtid, si);
    }
  } else if(WIFEXITED(status)) {
    std::cerr << "-> exited" << std::endl;
    on_process_exited(self, WEXITSTATUS(status));
  } else if(WIFSIGNALED(status)) {
    std::cerr << "-> signaled (sig = " << WTERMSIG(status) << ")" << std::endl;
    on_process_killed(self, WTERMSIG(status));
  } else {
    std::cerr << "-> unknown event" << std::endl;
  }
}

LocalDebuggee::Impl::thread_iterator LocalDebuggee::Impl::find_thread(thread_id tid)
{
  return std::find_if(threads_.begin(), threads_.end(),
    [tid](const LocalDebugThread & thr) { return thr.id() == tid; });
}

void LocalDebuggee::Impl::on_process_created(LocalDebuggee & self, thread_id which,
                                             process_id new_pid)
{
  auto& thr_which = get_thread(which);
  notify_process_created(self, thr_which, new_pid);
  if(::ptrace(PTRACE_DETACH, new_pid, nullptr, nullptr) == -1)
    throw std::system_error(errno, std::system_category());
}

void LocalDebuggee::Impl::on_process_exited(LocalDebuggee & self, int exit_code)
{
  notify_process_exited(self, exit_code);
  attached_ = false;
}

void LocalDebuggee::Impl::on_process_killed(LocalDebuggee & self, int sig)
{
  notify_process_killed(self, sig);
  attached_ = false;
}

void LocalDebuggee::Impl::on_thread_created(LocalDebuggee & self, thread_id which,
                                            thread_id new_tid)
{
  if(::waitpid(new_tid, nullptr, __WALL) == -1)
    throw std::system_error(errno, std::system_category());
  auto& new_thr = add_traced_thread(new_tid);
  
  auto& thr_which = get_thread(which);
  notify_thread_created(self, thr_which, new_thr);
}

void LocalDebuggee::Impl::on_thread_exited(LocalDebuggee & self, thread_id which,
                                           int exit_code)
{
  const auto thr_itr = find_thread(which);
  if(thr_itr == threads_.end())
    throw std::invalid_argument("unknown thread");
  
  notify_thread_exited(self, *thr_itr, exit_code);
  
  if(which != process_.id())
    threads_.erase(thr_itr);
}

void LocalDebuggee::Impl::on_sig_trap(LocalDebuggee & self, thread_id which,
                                      const siginfo_t & si)
{
  auto& thr_which = get_thread(which);
  if(si.si_signo == TRAP_TRACE) {
    notify_singlestep(self, thr_which);
  } else {
    if(self.bp_mgr_.dispatch_bp_hit(self, thr_which)) {
      self.singlestep(thr_which);
      self.bp_mgr_.restore_bps();
    } else {
      notify_unhandled_bp(self, thr_which);
      sig_deliv_ = evt_ign_ ? 0 : SIGTRAP;
    }
  }
}

void LocalDebuggee::Impl::on_signal(LocalDebuggee & self, thread_id which,
                                    const siginfo_t & si)
{
  auto& thr_which = get_thread(which);
  if(si.si_signo == SIGSEGV)
    notify_access_violation(self, thr_which);
  sig_deliv_ = evt_ign_ ? 0 : si.si_signo;
}

void LocalDebuggee::Impl::notify_process_created(LocalDebuggee & self, LocalDebugThread & which,
                                                 process_id new_pid)
{
  self.evt_emitter_.emit(ProcessCreatedEvent{ &self, &which, new_pid });
}

void LocalDebuggee::Impl::notify_process_exited(LocalDebuggee & self, int exit_code)
{
  self.evt_emitter_.emit(ProcessExitedEvent{ &self, exit_code });
}

void LocalDebuggee::Impl::notify_process_killed(LocalDebuggee & self, int term_sig)
{
  self.evt_emitter_.emit(ProcessKilledEvent{ &self, term_sig });
}

void LocalDebuggee::Impl::notify_thread_created(LocalDebuggee & self,
                                                LocalDebugThread & which,
                                                LocalDebugThread & thr_new)
{
  self.evt_emitter_.emit(ThreadCreatedEvent{ &self, &which, &thr_new });
}

void LocalDebuggee::Impl::notify_thread_exited(LocalDebuggee & self,
                                               LocalDebugThread & which,
                                               int exit_code)
{
  self.evt_emitter_.emit(ThreadExitedEvent{ &self, &which, exit_code });
}

void LocalDebuggee::Impl::notify_unhandled_bp(LocalDebuggee & self,
                                              LocalDebugThread & which)
{
  self.evt_emitter_.emit(UnhandledBpEvent{ &self, &which });
}

void LocalDebuggee::Impl::notify_singlestep(LocalDebuggee & self,
                                            LocalDebugThread & which)
{
  self.evt_emitter_.emit(SinglestepEvent{ &self, &which });
}

void LocalDebuggee::Impl::notify_access_violation(LocalDebuggee & self,
                                                  LocalDebugThread & which)
{
  self.evt_emitter_.emit(AccessViolationEvent{ &self, &which });
}

LocalDebuggee::LocalDebuggee(LocalDebugProcess && proc, int flags)
  : pimpl_( new Impl(*this, std::move(proc), flags) ) {}

LocalDebuggee::LocalDebuggee(LocalDebuggee &&) = default;

LocalDebuggee::~LocalDebuggee()
{
  if(pimpl_->attached_)
    std::terminate(); // detach or kill? can't decide
}

DebugProcess & LocalDebuggee::process()
{
  return pimpl_->process_;
}

const DebugProcess & LocalDebuggee::process() const
{
  return pimpl_->process_;
}

DebugThread & LocalDebuggee::get_thread(thread_id tid)
{
  return pimpl_->get_thread(tid);
}

const DebugThread & LocalDebuggee::get_thread(thread_id tid) const
{
  return pimpl_->get_thread(tid);
}

bool LocalDebuggee::attached() const
{
  return pimpl_->attached_;
}

void LocalDebuggee::singlestep(DebugThread & run_thr)
{
  ptrace_singlestep(run_thr.id(), 0);
  Impl::DebugEventInternal evt;
  pimpl_->wait_event(run_thr.id(), &evt);
  pimpl_->dispatch_event(*this, evt);
}

void LocalDebuggee::run()
{
  for(auto& dbg_thr : pimpl_->threads_) {
    if(::ptrace(PTRACE_CONT, dbg_thr.id(), nullptr, nullptr) == -1)
      throw std::system_error(errno, std::system_category());
  }
  
  while(pimpl_->attached_) {
    Impl::DebugEventInternal evt;
    pimpl_->wait_event(&evt);
    pimpl_->dispatch_event(*this, evt);
    
    if(pimpl_->attached_) {
      if(::ptrace(PTRACE_CONT, evt.wtid, nullptr, pimpl_->sig_deliv_) == -1)
        throw std::system_error(errno, std::system_category());
    }
  }
}

void LocalDebuggee::detach()
{
  for(auto& thr : pimpl_->threads_) {
    if(::ptrace(PTRACE_DETACH, thr.id(), nullptr, nullptr) == -1)
      throw std::system_error(errno, std::system_category());
  }
  pimpl_->attached_ = false;
}

void LocalDebuggee::kill()
{
  pimpl_->process_.kill(true);
  pimpl_->attached_ = false;
}

void LocalDebuggee::add_listener(std::shared_ptr<DebugEventListener> sp_listener)
{
  evt_emitter_.add_listener(std::move(sp_listener));
}

void LocalDebuggee::remove_listener(const std::shared_ptr<DebugEventListener> & sp_listener)
{
  evt_emitter_.remove_listener(sp_listener);
}

void LocalDebuggee::discard_event()
{
  pimpl_->evt_ign_ = true;
}

breakpoint_id LocalDebuggee::set_bp(Breakpoint * bp, BpHandlerFn fn)
{
  return bp_mgr_.set_bp(*this, bp, fn);
}

void LocalDebuggee::remove_bp(breakpoint_id bp_id)
{
  bp_mgr_.remove_bp(bp_id);
}

void LocalDebuggee::remove_all_bps()
{
  bp_mgr_.remove_all_bps();
}

std::unique_ptr<Debuggee> dbg_exec(const ExecParams & params, int flags)
{
  std::vector<const char *> arg_ptrs { params.file.c_str() };
  if(params.flags & ExecParams::Flags::HasArgs) {
    for(const auto& arg: params.args)
      arg_ptrs.push_back(arg.c_str());
  }
  arg_ptrs.push_back(nullptr);
  char ** const execve_argv = const_cast<char **>(arg_ptrs.data());
  
  char ** execve_envp = nullptr;
  std::vector<std::string> env_vars;
  std::vector<const char *> env_var_ptrs;
  if(params.flags & ExecParams::Flags::HasEnv) {
    for(const auto& envp: params.env) {
      const auto env_var = make_env_var(envp.first, envp.second);
      env_vars.emplace_back(std::move(env_var));
      env_var_ptrs.push_back(env_vars.back().c_str());
    }
    env_var_ptrs.push_back(nullptr);
    execve_envp = const_cast<char **>(env_var_ptrs.data());
  } else {
    execve_envp = ::environ;
  }
  
  const pid_t pid = ::fork();
  if(pid == -1)
    throw std::system_error(errno, std::system_category());
  
  if(pid == 0) { // child stub
    if(::ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) == -1)
      std::exit(errno);
    
    if(::raise(SIGSTOP) != 0)
      std::exit(errno);
    
    ::execve(execve_argv[0], execve_argv, execve_envp);
    std::exit(errno);
  }
  
  int status;
  if(::waitpid(pid, &status, WUNTRACED) == -1) // sigstop
    throw std::system_error(errno, std::system_category());
  if(WIFEXITED(status))
    throw std::system_error(WEXITSTATUS(status), std::system_category());
  
  if(::ptrace(PTRACE_CONT, pid, nullptr, nullptr) == -1)
    throw std::system_error(errno, std::system_category());
  if(::waitpid(pid, &status, 0) == -1) // sigtrap
    throw std::system_error(errno, std::system_category());
  if(WIFEXITED(status))
    throw std::system_error(WEXITSTATUS(status), std::system_category());
  
  ProcessKillGuard pk_guard( pid, SIGKILL );
  LocalDebugProcess dbg_proc( pid, DebugProcess::OpenFlags::AllAccess );
  std::unique_ptr<Debuggee> dbg_ptr( new LocalDebuggee(std::move(dbg_proc), flags) );
  pk_guard.cancel();
  return dbg_ptr;
}

std::unique_ptr<Debuggee> dbg_attach(process_id pid, int flags)
{
  ProcessStopGuard ps_guard( pid );
  
  // only attach to main thread
  if(::ptrace(PTRACE_ATTACH, pid, nullptr, nullptr) == -1)
    throw std::system_error(errno, std::system_category());
  
  if(::waitpid(pid, nullptr, 0) == -1)
    throw std::system_error(errno, std::system_category());
  
  LocalDebugProcess dbg_proc(pid, DebugProcess::OpenFlags::AllAccess);
  std::unique_ptr<Debuggee> dbg_instance( new LocalDebuggee(std::move(dbg_proc), flags) );
  return dbg_instance;
}

} // namespace hdbg
