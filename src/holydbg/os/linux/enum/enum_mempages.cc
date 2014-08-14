#include <holydbg/enum/enum_mempages.hpp>

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

namespace hdbg {

class MemPageRange::Impl
{
public:
  Impl(process_id);
  Impl(const Impl &);
  bool first(MemPageEntry &);
  bool next(MemPageEntry &);
  
private:
  void build_entry(MemPageEntry &);
  
  process_id pid_;
  std::ifstream procmaps_;
  std::string curline_;
};

MemPageRange::Impl::Impl(process_id pid)
  : pid_(pid)
  , procmaps_( std::string("/proc/") + std::to_string(pid) + "/maps" ) {}

MemPageRange::Impl::Impl(const Impl & imp) : Impl(imp.pid_) {}

bool MemPageRange::Impl::first(MemPageEntry & mpage_e)
{
  if(!std::getline(procmaps_, curline_))
    return false;
  
  build_entry(mpage_e);
  return true;
}

bool MemPageRange::Impl::next(MemPageEntry & mpage_e)
{
  if(!std::getline(procmaps_, curline_))
    return false;
  
  build_entry(mpage_e);
  return true;
}

void MemPageRange::Impl::build_entry(MemPageEntry & mpage_e)
{
  std::uintptr_t maps_from, maps_to;
  char sep;
  unsigned int maps_offs, maps_inode;
  std::string maps_rwxs, maps_dev, maps_pathname;
  
  std::istringstream iss(curline_);
  iss >> std::hex >> maps_from
      >> sep
      >> std::hex >> maps_to
      >> maps_rwxs
      >> maps_offs
      >> maps_dev
      >> maps_inode
      >> maps_pathname;
  
  mpage_e.base  = maps_from;
  mpage_e.size  = maps_to - maps_from;
  mpage_e.perms = ((maps_rwxs[0] == 'r') ? MemAccess::MemRead   : 0) |
                  ((maps_rwxs[1] == 'w') ? MemAccess::MemWrite  : 0) |
                  ((maps_rwxs[2] == 'x') ? MemAccess::MemExec   : 0) |
                  ((maps_rwxs[3] == 's') ? MemAccess::MemShared : 0) ;
  mpage_e.name  = maps_pathname;
}

MemPageRange::MemPageRange(process_id pid) : pimpl_( new Impl(pid) ) {}

MemPageRange::MemPageRange(const MemPageRange & mpage_rng)
  : pimpl_( new Impl(*mpage_rng.pimpl_) ) {}

MemPageRange::MemPageRange(MemPageRange &&) = default;
MemPageRange::~MemPageRange() = default;

MemPageIterator MemPageRange::begin()
{
  return MemPageIterator(*this);
}

MemPageIterator MemPageRange::end()
{
  return MemPageIterator();
}

MemPageIterator::MemPageIterator() : mpage_rng_(nullptr) {}

MemPageIterator::MemPageIterator(MemPageRange & mpage_rng)
  : mpage_rng_(&mpage_rng)
{
  if(!mpage_rng_->pimpl_->first(mpage_entry_))
    mpage_rng_ = nullptr;
}

void MemPageIterator::increment()
{
  if(mpage_rng_ && !mpage_rng_->pimpl_->next(mpage_entry_))
    mpage_rng_ = nullptr;
}

bool MemPageIterator::equal(const MemPageIterator & mpage_itr) const
{
  return mpage_rng_ == mpage_itr.mpage_rng_ &&
         (!mpage_rng_ || mpage_entry_ == mpage_itr.mpage_entry_);
}

const MemPageEntry & MemPageIterator::dereference() const
{
  assert(mpage_rng_);
  return mpage_entry_;
}

} // namespace hdbg
