#ifndef __HDBG_ENUM_ENUM_MEMPAGES_HPP__
#define __HDBG_ENUM_ENUM_MEMPAGES_HPP__

#include <hdbg/config.h>
#include <hdbg/sys_types.hpp>

#include <boost/iterator/iterator_facade.hpp>

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <string>

namespace hdbg {

enum MemAccess
{
  MemRead   = 1 << 0,
  MemWrite  = 1 << 1,
  MemExec   = 1 << 2,
  MemShared = 1 << 3,
};

struct MemPageEntry
{
  std::uintptr_t base;
  std::size_t size;
  unsigned int perms;
  std::string name;
};

inline bool operator==(const MemPageEntry & lhs, const MemPageEntry & rhs)
{
  return lhs.base  == rhs.base  &&
         lhs.size  == rhs.size  &&
         lhs.perms == rhs.perms &&
         lhs.name  == rhs.name  ;
}

inline bool operator!=(const MemPageEntry & lhs, const MemPageEntry & rhs)
{
  return !(lhs == rhs);
}

class MemPageRange;

class HDBG_API MemPageIterator
  : public boost::iterator_facade< MemPageIterator,
                                   const MemPageEntry,
                                   std::input_iterator_tag >
{
  friend class boost::iterator_core_access;
  
public:
  MemPageIterator();
  explicit MemPageIterator(MemPageRange &);
  
private:
  void increment();
  bool equal(const MemPageIterator &) const;
  const MemPageEntry & dereference() const;
  
  MemPageRange * mpage_rng_ = nullptr;
  MemPageEntry mpage_entry_;
};

class HDBG_API MemPageRange
{
  friend class MemPageIterator;
  
public:
  typedef MemPageIterator iterator;
  typedef MemPageIterator const_iterator;
  
  explicit MemPageRange(process_id);
  MemPageRange(const MemPageRange &);
  MemPageRange(MemPageRange &&);
  ~MemPageRange();
  
  iterator begin();
  iterator end();
  
private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

inline MemPageRange enum_mempages(process_id pid)
{
  return MemPageRange(pid);
}

} // namespace hdbg

#endif // __HDBG_ENUM_ENUM_MEMPAGES_HPP__
