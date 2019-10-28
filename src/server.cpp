#include "thread.h"

#include <mc_udp/server.h>

std::function<void()> rt_function()
{
  return [](){};
}
