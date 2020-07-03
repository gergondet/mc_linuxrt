/*
 * Copyright 2019-2020 mc_rtc development team
 */

#pragma once

#include <cstdint>

struct Timer
{
  /** Create a periodic timer with a duration in micro-seconds */
  Timer(unsigned int usec);

  void wait();

  uint64_t missed() const { return missed_; }
private:
  int fd_ = 0;
  uint64_t missed_ = 0;
};
