#include <gtest/gtest.h>

#include <hdbg/arch_services.hpp>
#include <hdbg/binary_format.hpp>
#include <hdbg/arch/code_tracer.hpp>
#include <hdbg/dbg/local/debuggee.hpp>
#include <hdbg/dbg/local/debug_process.hpp>

#include <cstdint>
#include <list>
#include <memory>
#include <stdexcept>

class TestTracer
  : public hdbg::CodeTracer
{
public:
  TestTracer();
  virtual ~TestTracer();
  
  virtual void * root_block() override;
  using CodeTracer::root_block;
  
  virtual void * add_block(std::uintptr_t from, std::uintptr_t to) override;
  virtual void remove_block(void * block) override;
  virtual void link_block(void * parent, void * child) override;
  
  virtual void * get_block(std::uintptr_t addr) override;
  using CodeTracer::get_block;
  
  virtual std::uintptr_t block_begin(const void * block) const override;
  virtual std::uintptr_t block_end(const void * block) const override;
  
private:
  
  struct BlockData
  {
    std::uintptr_t begin, end;
    std::vector<void *> childs;
  };
  
  std::list<BlockData> blocks_;
};

TestTracer::TestTracer() = default;
TestTracer::~TestTracer() = default;

void * TestTracer::root_block()
{
  return reinterpret_cast<void *>(-1);
}

void * TestTracer::add_block(std::uintptr_t from, std::uintptr_t to)
{
  if(from > to)
    throw std::invalid_argument("invalid block dimensions");
  
  const auto itr = std::find_if(blocks_.begin(), blocks_.end(),
    [from, to] (BlockData & bdata) {
      return ( bdata.begin >= from && bdata.begin < to ) ||
             ( bdata.end   >= from && bdata.end   < to ) ||
             ( from >= bdata.begin && from < bdata.end ) ||
             ( to   >= bdata.begin && to   < bdata.end ) ;
    });
  
  if(itr != blocks_.end()) {
    if(from < itr->begin || to > itr->end)
      throw std::logic_error("overlapped blocks");
    return &*itr;
  }
  
  blocks_.emplace_back(BlockData{ from, to - from });
  return &blocks_.back();
}

void TestTracer::remove_block(void * block)
{
  const auto itr = std::find_if(blocks_.begin(), blocks_.end(),
    [block] (BlockData & bdata) { return &bdata == block; });
  
  if(itr != blocks_.end())
    blocks_.erase(itr);
}

void TestTracer::link_block(void * parent, void * child)
{
  if(parent == root_block())
    return;
  
  const auto bd_parent = static_cast<BlockData *>(parent);
  const auto bd_child = static_cast<BlockData *>(child);
  bd_parent->childs.push_back(bd_child);
}

void * TestTracer::get_block(std::uintptr_t addr)
{
  const auto itr = std::find_if(blocks_.begin(), blocks_.end(),
    [addr](BlockData & bdata) {
      return addr >= bdata.begin && addr < bdata.end;
    });
  return (itr != blocks_.end()) ? &*itr : nullptr;
}

std::uintptr_t TestTracer::block_begin(const void * block) const
{
  return (block != root_block()) ? static_cast<const BlockData *>(block)->begin : 0;
}

std::uintptr_t TestTracer::block_end(const void * block) const
{
  return (block != root_block()) ? static_cast<const BlockData *>(block)->end : 0;
}

class TracerTest
  : public ::testing::Test
{
public:
  TracerTest();
  virtual ~TracerTest();
  
  std::unique_ptr<hdbg::LocalDebuggee> debuggee_;
  hdbg::LocalDebugProcess & dbg_proc_;
  hdbg::BinaryFormat & proc_img_;
  hdbg::ArchServices & arch_svc_;
  TestTracer tracer_;
};

TracerTest::TracerTest()
  : debuggee_([]{
      hdbg::DbgExecParams ep;
      ep.file = "dummy_exec";
      return hdbg::LocalDebuggee::exec(ep);
    }())
  , dbg_proc_( debuggee_->process() )
  , proc_img_( dbg_proc_.image() )
  , arch_svc_( hdbg::get_arch_services(proc_img_.arch()) ) {}

TracerTest::~TracerTest()
{
  debuggee_->kill();
}

TEST_F(TracerTest, TestTraceEngine) {
  const auto block_fst = tracer_.add_block(0, 10);
  EXPECT_EQ(block_fst, tracer_.get_block(5));
  const auto block_snd = tracer_.add_block(5, 8);
  EXPECT_EQ(block_fst, block_snd);
  EXPECT_THROW(tracer_.add_block(9, 11), std::logic_error);
}

TEST_F(TracerTest, ArchServicesRunTrace) {
  
}

