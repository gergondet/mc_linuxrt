/*
 * Copyright 2019-2020 mc_rtc development team
 */

#include "thread.h"

#include <chrono>
#include <iostream>
#include <thread>

namespace app
{

using clock = std::chrono::steady_clock;
using time_point = std::chrono::time_point<clock>;

struct LatencyMeasure
{
  LatencyMeasure(uint64_t usec)
  : usec_(usec), nMeasures_(1000000 / usec_)
  {
  }

  void run()
  {
    while(MC_LINUXRT_RUN)
    {
      auto now = clock::now();
      auto delay = std::chrono::duration_cast<std::chrono::microseconds>(now - prev_);
      avg_ += delay.count();
      max_ = std::max<double>(delay.count(), max_);
      if(delay.count() > 1.1 * usec_)
      {
        missed_ += 1;
      }
      prev_ = now;
      if(iter_++ % nMeasures_ == 0)
      {
        std::cout << "Average delay for the last " << nMeasures_ << " iterations: " << (avg_ / nMeasures_) << "\n";
        std::cout << "Worst case: " << max_ << "\n";
        std::cout << "Missed: " << missed_ << "\n";
        max_ = 0;
        avg_ = 0;
        missed_ = 0;
      }
      sched_yield();
    }
  }

  time_point prev_ = clock::now();
  unsigned int usec_ = 0;
  double avg_ = 0;
  double max_ = 0;
  size_t missed_ = 0;
  size_t iter_ = 0;
  size_t nMeasures_ = 1000;
};

} // namespace app

void * init(int, char * [], uint64_t & cycle_ns)
{
  return new app::LatencyMeasure(cycle_ns / 1000);
}

void run(void * data)
{
  static_cast<app::LatencyMeasure *>(data)->run();
}
