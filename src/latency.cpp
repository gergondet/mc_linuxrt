#include "thread.h"
#include "timer.h"

#include <chrono>
#include <iostream>
#include <thread>

using hr_clock = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<hr_clock>;

struct LatencyMeasure
{
  LatencyMeasure(unsigned int usec)
  : timer_(usec), nMeasures_(1000000 / usec)
  {
  }

  void run()
  {
    while(MC_LINUXRT_RUN)
    {
      auto now = hr_clock::now();
      auto delay = std::chrono::duration_cast<std::chrono::microseconds>(now - prev_);
      avg_ += delay.count();
      max_ = std::max<double>(delay.count(), max_);
      prev_ = now;
      if(iter_++ % nMeasures_ == 0)
      {
        std::cout << "Average delay for the last " << nMeasures_ << " iterations: " << (avg_ / nMeasures_) << "\n";
        std::cout << "Worst case: " << max_ << "\n";
        std::cout << "Missed until now: " <<timer_.missed() << "\n";
        max_ = 0;
        avg_ = 0;
      }
      timer_.wait();
    }
  }

  time_point prev_ = hr_clock::now();
  Timer timer_;
  double avg_ = 0;
  double max_ = 0;
  size_t iter_ = 0;
  size_t nMeasures_ = 1000;
};

std::function<void()> rt_function(int argc, char * argv[])
{
  LatencyMeasure lm(argc > 1 ? std::atoi(argv[1]) : 1000);
  return [lm]() mutable { lm.run(); };
}
