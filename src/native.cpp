/*
 * Copyright 2019-2020 mc_rtc development team
 */

#include "thread.h"

#include <mc_control/mc_global_controller.h>

void * init(int argc, char * argv[], uint64_t & dt)
{
  auto gc = new mc_control::MCGlobalController(argc > 1 ? argv[1] : mc_rtc::CONF_PATH);
  gc->init({0});
  gc->running = true;
  dt = std::ceil(gc->timestep() * 1e9);
  return gc;
}

void run(void * data)
{
  auto gc = static_cast<mc_control::MCGlobalController *>(data);
  while(MC_LINUXRT_RUN)
  {
    MC_LINUXRT_RUN = gc->run();
    sched_yield();
  }
}
