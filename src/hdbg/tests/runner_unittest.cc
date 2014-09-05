#include <gtest/gtest.h>

#include "../utils/runner.hpp"

#include <exception>
#include <thread>

class RunnerTest
  : public ::testing::Test
{
public:
  RunnerTest();
  virtual ~RunnerTest();
  
  hdbg::Runner runner_;
};

RunnerTest::RunnerTest() = default;
RunnerTest::~RunnerTest() = default;

TEST_F(RunnerTest, RunFn) {
  unsigned int run_fn = 0;
  runner_.run([&] { ++run_fn; });
  ASSERT_EQ(1, run_fn);
}

TEST_F(RunnerTest, SameThread) {
  unsigned int run_fn = 0;
  std::thread::id ids[2];
  runner_.run([&] {
    ++run_fn;
    ids[0] = std::this_thread::get_id();
  });
  runner_.run([&] {
    ++run_fn;
    ids[1] = std::this_thread::get_id();
  });
  ASSERT_EQ(2, run_fn);
  ASSERT_EQ(ids[0], ids[1]);
}

TEST_F(RunnerTest, ReturnValue) {
  const unsigned int n = runner_.run([] { return 1337; });
  ASSERT_EQ(1337, n);
}

TEST_F(RunnerTest, ThrowException) {
  ASSERT_THROW({
    runner_.run([] { throw std::exception(); });
  }, std::exception);
}
