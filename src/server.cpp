/*
 * Copyright 2019-2020 mc_rtc development team
 */

#include "thread.h"

#include <mc_udp/server/Server.h>

#include <iostream>

void * init(int argc, char * argv[], uint64_t &)
{
  mc_udp::Server * server = new mc_udp::Server(argc > 1 ? std::atoi(argv[1]) : 4444);
  return server;
}

void run(void * data)
{
  static bool got_control_ = false;
  static bool lost_control_ = false;
  static size_t lost_control_iter_ = 0;
  auto server = static_cast<mc_udp::Server *>(data);
  while(MC_LINUXRT_RUN)
  {
    // Actual code should send command to motor
    if(server->recv())
    {
      got_control_ = true;
      if(lost_control_)
      {
        lost_control_ = false;
        std::cerr << "Control recovered after " << lost_control_iter_ << " lost iterations\n";
      }
    }
    else
    {
      if(got_control_ && !lost_control_)
      {
        std::cerr << "Lost control\n";
        lost_control_ = true;
        got_control_ = false;
      }
      if(lost_control_)
      {
        lost_control_iter_ += 1;
      }
    }
    // Actual code should read the sensors
    server->sensors().encoders.resize(1);
    if(got_control_)
    {
      server->sensors().encoders[0] = server->control().encoders[0];
    }
    else
    {
      server->sensors().encoders[0] = 0;
    }
    server->sensors().id += 1;
    server->send();
    sched_yield();
  }
}
