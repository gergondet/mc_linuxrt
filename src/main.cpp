#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include <iostream>

#include "thread.h"

int main(int argc, char * argv[])
{
  struct sched_param param;
  pthread_attr_t attr;
  pthread_t thread;
  int ret;

  /* Lock memory */
  if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1)
  {
    std::cerr << "mlockall failed: " << strerror(errno) << "\n";
    exit(-2);
  }

  /* Initialize pthread attributes (default values) */
  ret = pthread_attr_init(&attr);
  if (ret)
  {
    std::cerr << "init pthread attributes failed\n";
    return ret;
  }

  /* Set a specific stack size  */
  ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
  if (ret)
  {
    std::cerr << "pthread setstacksize failed\n";
    return ret;
  }

  /* Set scheduler policy and priority of pthread */
  ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  if (ret)
  {
    std::cerr << "pthread setschedpolicy failed\n";
    return ret;
  }
  param.sched_priority = 80;
  ret = pthread_attr_setschedparam(&attr, &param);
  if (ret)
  {
    std::cerr << "pthread setschedparam failed\n";
    return ret;
  }
  /* Use scheduling parameters of attr */
  ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  if (ret)
  {
    std::cerr << "pthread setinheritsched failed\n";
    return ret;
  }

  auto thread_func = rt_function();

  /* Create a pthread with specified attributes */
  ret = pthread_create(&thread, &attr, thread_func, NULL);
  if (ret)
  {
    std::cerr << "create pthread failed\n";
    return ret;
  }

  /* Join the thread and wait until it is done */
  ret = pthread_join(thread, NULL);
  if (ret)
  {
    std::cerr << "join pthread failed: %m\n";
    return ret;
  }

  return 0;
}
