#include <holydbg/enum/enum_threads.hpp>

#include <boost/filesystem.hpp>

#include <cassert>
#include <string>

namespace hdbg {

namespace {

namespace fs = boost::filesystem;
using DThrIterator = fs::directory_iterator;

} // namespace

class ThreadRange::Impl
{
public:
  Impl(process_id);
  bool first(ThreadEntry &);
  bool next(ThreadEntry &);
  
private:
  void build_entry(ThreadEntry &);
  
  DThrIterator dthr_itr_;
};

ThreadRange::Impl::Impl(process_id pid)
  : dthr_itr_( std::string("/proc/") + std::to_string(pid) + "/task" ) {}

bool ThreadRange::Impl::first(ThreadEntry & thr_e)
{
  const DThrIterator dthr_end;
  if(dthr_itr_ == dthr_end)
    return false;
  
  build_entry(thr_e);
  return true;
}

bool ThreadRange::Impl::next(ThreadEntry & thr_e)
{
  const DThrIterator dthr_end;
  if(++dthr_itr_ == dthr_end)
    return false;
  
  build_entry(thr_e);
  return true;
}

void ThreadRange::Impl::build_entry(ThreadEntry & thr_e)
{
  const std::string thr_dname = dthr_itr_->path().filename().string();
  thr_e.tid = std::stol(thr_dname);
}

ThreadRange::ThreadRange(process_id pid) : pimpl_( new Impl(pid) ) {}

ThreadRange::ThreadRange(const ThreadRange & thr_rng)
  : pimpl_( new Impl(*thr_rng.pimpl_) ) {}

ThreadRange::ThreadRange(ThreadRange &&) = default;

ThreadRange::~ThreadRange() = default;

ThreadIterator ThreadRange::begin()
{
  return ThreadIterator(*this);
}

ThreadIterator ThreadRange::end()
{
  return ThreadIterator();
}

ThreadIterator::ThreadIterator() : thr_rng_(nullptr) {}

ThreadIterator::ThreadIterator(ThreadRange & thr_rng)
  : thr_rng_(&thr_rng)
{
  if(!thr_rng_->pimpl_->first(thr_entry_))
    thr_rng_ = nullptr;
}

void ThreadIterator::increment()
{
  if(thr_rng_ && !thr_rng_->pimpl_->next(thr_entry_))
    thr_rng_ = nullptr;
}

bool ThreadIterator::equal(const ThreadIterator & thr_itr) const
{
  return thr_rng_ == thr_itr.thr_rng_ &&
         (!thr_rng_ || thr_entry_ == thr_itr.thr_entry_);
}

const ThreadEntry & ThreadIterator::dereference() const
{
  assert(thr_rng_);
  return thr_entry_;
}

} // namespace hdbg
