#include <gtest/gtest.h>

#include <holydbg/this_process.hpp>
#include <holydbg/enum/enum_threads.hpp>

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <mutex>
#include <thread>
#include <vector>

TEST(EnumThrTest, EnumSelfThreads) {
  std::mutex thr_mtx;
  std::unique_lock<std::mutex> thr_mlock(thr_mtx);
  
  const unsigned int nthreads = std::rand() % 30;
  std::vector<std::thread> all_threads;
  all_threads.reserve(nthreads);
  for(int i = 0; i < nthreads; ++i) {
    all_threads.emplace_back([&thr_mtx] {
      std::unique_lock<std::mutex> thr_lock(thr_mtx);
    });
  }
  
  auto self_threads = hdbg::enum_threads( hdbg::this_process::id() );
  unsigned int thr_count = std::distance(self_threads.begin(), self_threads.end());
  
  thr_mlock.unlock();
  std::for_each(all_threads.begin(), all_threads.end(), std::mem_fn(&std::thread::join));
  ASSERT_EQ(all_threads.size() + 1, thr_count);
}

