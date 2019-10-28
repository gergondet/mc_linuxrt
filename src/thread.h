#pragma once

#include <functional>

// This function should return a functor. This functor will run in an RT thread
std::function<void()> rt_function();
