#include "thread.h"

#include <chrono>
#include <iostream>
#include <thread>

using hr_clock = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<hr_clock>;

struct LatencyMeasure
{
  void run()
  {
    while(MC_LINUXRT_RUN)
    {
      auto now = hr_clock::now();
      auto delay = std::chrono::duration_cast<std::chrono::microseconds>(now - prev);
      avg_ += delay.count();
      max_ = std::max<double>(delay.count(), max_);
      prev = now;
      if(iter_++ % nMeasures_ == 0)
      {
        std::cout << "Average delay for the last " << nMeasures_ << " iterations: " << (avg_ / nMeasures_) << "\n";
        std::cout << "Worst case: " << max_ << "\n";
        max_ = 0;
        avg_ = 0;
      }
      std::this_thread::sleep_until(now + std::chrono::milliseconds(1));
    }
  }

  time_point prev = hr_clock::now();
  double avg_ = 0;
  double max_ = 0;
  size_t iter_ = 0;
  size_t nMeasures_ = 1000;
};

std::function<void()> rt_function()
{
  LatencyMeasure lm;
  return [lm]() mutable { lm.run(); };
}
