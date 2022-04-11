/*
 * Copyright 2019-2020 mc_rtc development team
 */

#include "thread.h"

#include <mc_control/mc_global_controller.h>

void * init(int argc, char * argv[], uint64_t & dt)
{
  auto gc = new mc_control::MCGlobalController(argc > 1 ? argv[1] : "");
  std::vector<double> encoders(gc->robot().refJointOrder().size(), 0.0);
  gc->init(encoders);
  gc->running = true;
  dt = std::ceil(gc->timestep() * 1e9);
  return gc;
}

void run(void * data)
{
  auto gc = static_cast<mc_control::MCGlobalController *>(data);
  double delay = 0.0;
  gc->controller().logger().addLogEntry("perf_Delay", [&]() { return delay; });
  timespec tv;
  clock_gettime(CLOCK_REALTIME, &tv);
  double prev = tv.tv_sec * 1000 + tv.tv_nsec * 1e-6;
  while(MC_LINUXRT_RUN)
  {
    clock_gettime(CLOCK_REALTIME, &tv);
    double now = tv.tv_sec * 1000 + tv.tv_nsec * 1e-6;
    delay = now - prev;
    prev = now;
    MC_LINUXRT_RUN = gc->run();
    sched_yield();
  }
}
