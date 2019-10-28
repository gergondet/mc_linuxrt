#include "timer.h"

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/timerfd.h>
#include <unistd.h>

Timer::Timer(unsigned int usec)
{
  fd_ = timerfd_create(CLOCK_MONOTONIC, 0);
  if(fd_ == -1)
  {
    std::cerr << "timerfd_create failed: " << strerror(errno) << "\n";
    throw(std::runtime_error("timerfd_create failed"));
  }
  struct itimerspec itval;
  itval.it_interval.tv_sec  = usec / 1000000;
  itval.it_interval.tv_nsec = 1000 * (usec % 1000000);
  itval.it_value.tv_sec  = usec / 1000000;
  itval.it_value.tv_nsec = 1000 * (usec % 1000000);
  if(timerfd_settime(fd_, 0, &itval, nullptr) != 0)
  {
    std::cerr << "timerfd_settime failed: " << strerror(errno) << "\n";
    throw(std::runtime_error("timerfd_settime failed"));
  }
}

void Timer::wait()
{
  uint64_t missed;
  if(read(fd_, &missed, sizeof(missed)) < 0)
  {
    std::cerr << "read failed: " << strerror(errno) << "\n";
    return;
  }
  if(missed != 1)
  {
    assert(missed > 0);
    missed_ += (missed - 1);
  }
}
