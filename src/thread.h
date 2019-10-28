#pragma once

#include <functional>

static bool MC_LINUXRT_RUN = true;

// This function should return a functor. This functor will run in an RT thread
std::function<void()> rt_function();
