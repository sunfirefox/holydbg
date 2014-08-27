#ifndef __HDBG_ENUM_ENUM_PROCESSES_HPP__
#define __HDBG_ENUM_ENUM_PROCESSES_HPP__

#include <hdbg/config.h>
#include <hdbg/sys_types.hpp>

#include <boost/iterator/iterator_facade.hpp>

#include <memory>
#include <string>

namespace hdbg {

struct ProcessEntry
{
  process_id pid;
  std::string name;
};

inline bool operator==(const ProcessEntry & lhs, const ProcessEntry & rhs)
{
  return lhs.pid  == rhs.pid  &&
         lhs.name == rhs.name ;
}

inline bool operator!=(const ProcessEntry & lhs, const ProcessEntry & rhs)
{
  return !(lhs == rhs);
}

class ProcessRange;

class HDBG_API ProcessIterator
  : public boost::iterator_facade< ProcessIterator,
                                   const ProcessEntry,
                                   std::input_iterator_tag >
{
  friend class boost::iterator_core_access;
  
public:
  ProcessIterator();
  explicit ProcessIterator(ProcessRange &);
  
private:
  void increment();
  bool equal(const ProcessIterator &) const;
  const ProcessEntry& dereference() const;
  
  ProcessRange * proc_rng_ = nullptr;
  ProcessEntry proc_entry_;
};

class HDBG_API ProcessRange
{
  friend class ProcessIterator;
  
public:
  typedef ProcessIterator iterator;
  typedef ProcessIterator const_iterator;
  
  ProcessRange();
  ProcessRange(const ProcessRange &);
  ProcessRange(ProcessRange &&);
  ~ProcessRange();
  
  iterator begin();
  iterator end();
  
private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

inline ProcessRange enum_processes()
{
  return ProcessRange();
}

} // namespace hdbg

#endif // __HDBG_PROCESS_ITERATOR_HPP__
