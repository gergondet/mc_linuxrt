#include "thread.h"

#include <mc_udp/server/Server.h>

std::function<void()> rt_function()
{
  return [](){};
}
