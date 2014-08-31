#include <gtest/gtest.h>

#include <hdbg/arch_services.hpp>
#include <hdbg/binary_format.hpp>
#include <hdbg/arch/code_tracer.hpp>
#include <hdbg/dbg/local/debuggee.hpp>
#include <hdbg/dbg/local/debug_process.hpp>

#include <cstdint>
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
  virtual void * add_block(std::uintptr_t addr) override;
  virtual void link_block(void * parent, void * child) override;
  
  virtual void * get_block(std::uintptr_t addr) override;
  using CodeTracer::get_block;
  
  virtual std::uintptr_t block_begin(const void * block) const override;
  virtual std::uintptr_t block_end(const void * block) const override;
  
private:
  
};

TestTracer::TestTracer() = default;
TestTracer::~TestTracer() = default;

void * TestTracer::root_block()
{
  
}

void * TestTracer::add_block(std::uintptr_t from, std::uintptr_t to)
{
  if(from > to)
    throw std::invalid_argument("invalid block dimensions");
  
}

void * TestTracer::add_block(std::uintptr_t addr)
{
  
}

void TestTracer::link_block(void * parent, void * child)
{
  
}

void * TestTracer::get_block(std::uintptr_t addr)
{
  
}

std::uintptr_t TestTracer::block_begin(const void * block) const
{
  
}

std::uintptr_t TestTracer::block_end(const void * block) const
{
  
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
}

TEST_F(TracerTest, ArchServicesRunTrace) {
  
}

