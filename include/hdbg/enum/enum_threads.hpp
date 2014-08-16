#ifndef __HDBG_ENUM_ENUM_THREADS_HPP__
#define __HDBG_ENUM_ENUM_THREADS_HPP__

#include <hdbg/config.h>
#include <hdbg/sys_types.hpp>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

#include <iterator>
#include <memory>

namespace hdbg {

struct ThreadEntry
{
  thread_id tid;
};

inline bool operator==(const ThreadEntry & lhs, const ThreadEntry & rhs)
{
  return lhs.tid == rhs.tid;
}

inline bool operator!=(const ThreadEntry & lhs, const ThreadEntry & rhs)
{
  return !(lhs == rhs);
}

class ThreadIterator;

class HDBG_EXPORT ThreadRange
{
  friend class ThreadIterator;
  
public:
  typedef ThreadIterator iterator;
  typedef ThreadIterator const_iterator;
  
  explicit ThreadRange(process_id);
  ThreadRange(const ThreadRange &);
  ThreadRange(ThreadRange &&);
  ~ThreadRange();
  
  iterator begin();
  iterator end();
  
private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

class HDBG_EXPORT ThreadIterator
  : public boost::iterator_facade< ThreadIterator,
                                   const ThreadEntry,
                                   std::input_iterator_tag >
{
  friend class boost::iterator_core_access;
  
public:
  ThreadIterator();
  explicit ThreadIterator(ThreadRange &);
  
private:
  void increment();
  bool equal(const ThreadIterator &) const;
  const ThreadEntry& dereference() const;
  
  const ThreadRange * thr_rng_ = nullptr;
  ThreadEntry thr_entry_;
};

inline ThreadRange enum_threads(process_id pid)
{
  return ThreadRange(pid);
}

} // namespace hdbg

#endif // __HDBG_ENUM_ENUM_THREADS_HPP__
