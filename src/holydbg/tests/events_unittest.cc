#include <gtest/gtest.h>

#include <holydbg/utils/event_emitter.hpp>
#include <holydbg/utils/event_listener.hpp>

#include <memory>

namespace {

struct FooEvent {};
struct BarEvent {};

using FooBarEmitter = hdbg::EventEmitter<FooEvent, BarEvent>;

class FooBarListener final
  : public hdbg::EventListener<FooEvent, BarEvent>
{
public:
  FooBarListener() : called_( false ) {}
  virtual ~FooBarListener() = default;
  
  bool called() const
  {
    return called_;
  }
  
  void reset()
  {
    called_ = false;
  }
  
private:
  virtual void handle_event(const FooEvent & ) override
  {
    called_ = true;
  }
  
  virtual void handle_event(const BarEvent & ) override
  {
    called_ = true;
  }
  
  bool called_;
};

} // namespace

TEST(EventTest, AddEventListener) {
  FooBarEmitter emitter;
  auto listener = std::make_shared<FooBarListener>();
  emitter.add_listener(listener);
  
  emitter.emit(FooEvent{});
  ASSERT_TRUE(listener->called());
  
  listener->reset();
  ASSERT_FALSE(listener->called());
  
  emitter.emit(BarEvent{});
  EXPECT_TRUE(listener->called());
}

TEST(EventTest, RemoveEventListener) {
  FooBarEmitter emitter;
  auto listener = std::make_shared<FooBarListener>();
  emitter.add_listener(listener);
  emitter.remove_listener(listener);
  
  emitter.emit(FooEvent{});
  ASSERT_FALSE(listener->called());
}

