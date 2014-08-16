#include <hdbg/enum/enum_processes.hpp>

#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp>

#include <cctype>
#include <fstream>
#include <iterator>
#include <string>

namespace hdbg {

namespace fs = boost::filesystem;

namespace {

struct is_procdir_process
{
  bool operator()(const fs::directory_entry & dir_entry) const
  {
    const std::string proc_fname = dir_entry.path().filename().string();
    return std::isdigit(proc_fname[0]) && fs::exists(dir_entry.path() / "task");
  }
};

std::string proc_name(process_id pid)
{
  const fs::path comm_path = fs::path("/proc") / std::to_string(pid) / "comm";
  std::ifstream comm_is ( comm_path.string() );
  std::string pcomm { std::istreambuf_iterator<char>(comm_is), 
                      std::istreambuf_iterator<char>() };
  pcomm.pop_back(); // remove trailing newline
  return pcomm;
}

using DProcIterator = boost::filter_iterator< is_procdir_process,
                                              fs::directory_iterator >;

} // namespace

class ProcessRange::Impl
{
public:
  Impl();
  bool first(ProcessEntry &);
  bool next(ProcessEntry &);
  
private:
  void build_entry(ProcessEntry &);
  DProcIterator dproc_itr_;
};

ProcessRange::Impl::Impl() : dproc_itr_( fs::directory_iterator("/proc") ) {}

bool ProcessRange::Impl::first(ProcessEntry & proc_e)
{
  const DProcIterator dproc_end;
  if(dproc_itr_ == dproc_end)
    return false;
  
  build_entry(proc_e);
  return true;
}

bool ProcessRange::Impl::next(ProcessEntry & proc_e)
{
  const DProcIterator dproc_end;
  if(++dproc_itr_ == dproc_end)
    return false;
  
  build_entry(proc_e);
  return true;
}

void ProcessRange::Impl::build_entry(ProcessEntry & proc_e)
{
  const std::string proc_dname = dproc_itr_->path().filename().string();
  proc_e.pid  = std::stol(proc_dname);
  proc_e.name = proc_name(proc_e.pid);
}

ProcessRange::ProcessRange() : pimpl_( new Impl ) {}

ProcessRange::ProcessRange(const ProcessRange &)
  : pimpl_( new Impl(*pimpl_) ) {}

ProcessRange::ProcessRange(ProcessRange &&) = default;
ProcessRange::~ProcessRange() = default;

ProcessIterator ProcessRange::begin()
{
  return ProcessIterator(*this);
}

ProcessIterator ProcessRange::end()
{
  return ProcessIterator();
}

ProcessIterator::ProcessIterator() : proc_rng_(nullptr) {}

ProcessIterator::ProcessIterator(ProcessRange & proc_rng)
  : proc_rng_( &proc_rng )
{
  if(!proc_rng_->pimpl_->first(proc_entry_))
    proc_rng_ = nullptr;
}

void ProcessIterator::increment()
{
  if(proc_rng_ && !proc_rng_->pimpl_->next(proc_entry_)) {
    proc_rng_ = nullptr;
  }
}

bool ProcessIterator::equal(const ProcessIterator & proc_itr) const
{
  return proc_rng_ == proc_itr.proc_rng_ &&
         (!proc_rng_ || proc_entry_ == proc_itr.proc_entry_);
}

const ProcessEntry & ProcessIterator::dereference() const
{
  assert(proc_rng_);
  return proc_entry_;
}

} // namespace hdbg
