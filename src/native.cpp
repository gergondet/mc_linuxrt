#include "thread.h"

#include <mc_control/mc_global_controller.h>

void * init(int argc, char * argv[], uint64_t)
{
  auto gc = new mc_control::MCGlobalController(argc > 1 ? argv[1] : mc_rtc::CONF_PATH);
  gc->init({0});
  gc->running = true;
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
