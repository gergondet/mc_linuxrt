#include "thread.h"

#include <mc_control/mc_global_controller.h>

std::function<void()> rt_function(int, char * [])
{
  return [](){};
}
