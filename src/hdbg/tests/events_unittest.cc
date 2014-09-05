#include <gtest/gtest.h>

#include <hdbg/utils/event_emitter.hpp>
#include <hdbg/utils/event_listener.hpp>

#include <memory>

struct FooEvent {};
struct BarEvent {};

using FooBarEmitter = hdbg::EventEmitter<FooEvent, BarEvent>;

class FooBarListener final
  : public hdbg::EventListener<FooEvent, BarEvent>
{
public:
  FooBarListener();
  virtual ~FooBarListener();
  
  virtual void on(const FooEvent & evt) override;
  virtual void on(const BarEvent & evt) override;
  
  bool called_;
};

class EventTest
  : public ::testing::Test
{
public:
  EventTest();
  virtual ~EventTest();
  
  FooBarEmitter emitter_;
  std::shared_ptr<FooBarListener> listener_;
};

EventTest::EventTest()
  : listener_( std::make_shared<FooBarListener>() ) {}

EventTest::~EventTest() = default;

TEST_F(EventTest, AddEventListener) {
  emitter_.add_listener(listener_);
  emitter_.emit_event(FooEvent{});
  EXPECT_TRUE(listener_->called_);
  
  listener_->called_ = false;
  emitter_.emit_event(BarEvent{});
  EXPECT_TRUE(listener_->called_);
}

TEST_F(EventTest, RemoveEventListener) {
  emitter_.add_listener(listener_);
  emitter_.remove_listener(listener_);
  emitter_.emit_event(FooEvent{});
  EXPECT_FALSE(listener_->called_);
}

FooBarListener::FooBarListener()
  : called_( false ) {}

FooBarListener::~FooBarListener() = default;

void FooBarListener::on(const FooEvent & )
{
  called_ = true;
}

void FooBarListener::on(const BarEvent & )
{
  called_ = true;
}
