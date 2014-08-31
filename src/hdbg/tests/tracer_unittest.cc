#include <gtest/gtest.h>

#include <hdbg/arch_services.hpp>
#include <hdbg/binary_format.hpp>
#include <hdbg/arch/code_tracer.hpp>
#include <hdbg/dbg/local/debuggee.hpp>
#include <hdbg/dbg/local/debug_process.hpp>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

class TestTracer
  : public hdbg::CodeTracer
{
public:
  TestTracer();
  virtual ~TestTracer();
  
  virtual void * root_block() override;
  using CodeTracer::root_block;
  
  virtual void * add_block(std::uintptr_t from, std::uintptr_t to) override;
  virtual void * add_block(std::uintptr_t addr) override;
  virtual void link_block(void * parent, void * child) override;
  
  virtual void * get_block(std::uintptr_t addr) override;
  using CodeTracer::get_block;
  
  virtual std::uintptr_t block_begin(const void * block) const override;
  virtual std::uintptr_t block_end(const void * block) const override;
  
private:
  
  class TraceBlock
  {
  public:
    TraceBlock(std::uintptr_t beg, std::uintptr_t end);
    
    void * add_block(std::uintptr_t from, std::uintptr_t to);
    void * add_block(std::uintptr_t addr);
    
    void * get_block(std::uintptr_t addr);
    
    std::uintptr_t begin, end;
    std::vector<TraceBlock *> childs;
    std::unique_ptr<TraceBlock> left, right;
  };
  
  TraceBlock root_;
};

bool test_tracer_engine_ok();

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

TEST_F(TracerTest, ArchServicesRunTrace) {
  ASSERT_TRUE( test_tracer_engine_ok() );
  
  const auto entry_pt = proc_img_.entry_point();
  for(const auto& sect : proc_img_.sections()) {
    if(sect.vaddr <= entry_pt && entry_pt < (sect.vaddr + sect.vsize)) {
      ASSERT_TRUE(sect.flags & hdbg::BinaryFormat::Section::Executable);
      
      const auto vbase = dbg_proc_.image_base();
      const auto tr_start = vbase + proc_img_.entry_point();
      const auto tr_size = sect.vaddr + sect.vsize - proc_img_.entry_point();
      
      TestTracer tracer;
      std::vector<std::uintptr_t> untraced;
      std::unique_ptr<std::uint8_t[]> sdata { new std::uint8_t[tr_size] };
      dbg_proc_.read_mem(tr_start, tr_size, sdata.get());
      arch_svc_.run_trace(tracer, tr_start, sdata.get(), tr_size, untraced);
    }
  }
}

TestTracer::TestTracer()
  : root_{ 0, 0 } {}

TestTracer::~TestTracer() = default;

void * TestTracer::root_block()
{
  return &root_;
}

void * TestTracer::add_block(std::uintptr_t from, std::uintptr_t to)
{
  std::cout << "add_block(" << (void *)from << "," << (void *)to << ")" << std::endl;
  
  if(from > to)
    throw std::invalid_argument("invalid block dimensions");
  
  return root_.add_block(from, to);
}

void * TestTracer::add_block(std::uintptr_t addr)
{
  std::cout << "add_block(" << (void *)addr << ")" << std::endl;
  
  return root_.add_block(addr);
}

void TestTracer::link_block(void * parent, void * child)
{
  assert(parent);
  assert(child);
  
  const auto tb_parent = static_cast<TraceBlock *>(parent);
  const auto tb_child = static_cast<TraceBlock *>(child);
  
  if(tb_parent == &root_)
    return;
  
  tb_parent->childs.push_back(tb_child);
}

void * TestTracer::get_block(std::uintptr_t addr)
{
  return root_.get_block(addr);
}

std::uintptr_t TestTracer::block_begin(const void * block) const
{
  assert(block);
  return static_cast<const TraceBlock *>(block)->begin;
}

std::uintptr_t TestTracer::block_end(const void * block) const
{
  assert(block);
  return static_cast<const TraceBlock *>(block)->end;
}

TestTracer::TraceBlock::TraceBlock(std::uintptr_t from, std::uintptr_t to)
  : begin( from ), end( to ) {}

void * TestTracer::TraceBlock::add_block(std::uintptr_t from, std::uintptr_t to)
{
  if(begin == from && end == to)
    return this;
  
  if(begin == end && from <= begin && begin < to) {
    begin = from;
    end = to;
    return this;
  }
  
  if(from < begin && to < end) {
    if(!left) {
      left.reset( new TraceBlock(from, to) );
      return left.get();
    } else {
      return left->add_block(from, to);
    }
  }
  
  if(from > begin && to > end) {
    if(!right) {
      right.reset( new TraceBlock(from, to) );
      return right.get();
    } else {
      return right->add_block(from, to);
    }
  }
  
  return nullptr;
}

void * TestTracer::TraceBlock::add_block(std::uintptr_t addr)
{
  return add_block(addr, addr);
}

void * TestTracer::TraceBlock::get_block(std::uintptr_t addr)
{
  if(begin < addr && addr < end)
    return this;
  
  if(addr < begin && addr < end)
    return left ? left->get_block(addr) : nullptr;
  
  if(addr > begin && addr > end)
    return right ? right->get_block(addr) : nullptr;
}

bool test_tracer_engine_ok()
{
  TestTracer tracer;
  
  if(tracer.root_block() == nullptr)
    return false;
  
  if(tracer.get_block(5) != nullptr)
    return false;
  
  const auto block_fst = tracer.add_block(0, 10);
  if(tracer.block_begin(block_fst) != 0  ||
     tracer.block_end(block_fst)   != 10 )
  {
    return false;
  }
  
  if(tracer.get_block(5) != block_fst)
    return false;
  
  const auto block_snd = tracer.add_block(13);
  if(tracer.add_block(11, 15) != block_snd)
    return false;
  
  return true;
}
