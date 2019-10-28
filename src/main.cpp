#include <linux/sched.h>
#include <linux/sched/types.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include "thread.h"

int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags)
{
  return syscall(__NR_sched_setattr, pid, attr, flags);
}

int main(int argc, char * argv[])
{
  struct sched_attr attr;

  /* Lock memory */
  if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1)
  {
    printf("mlockall failed: %m\n");
    return -2;
  }

  /* Configure deadline policy */
  memset(&attr, 0, sizeof(attr));
  attr.size = sizeof(attr);

  /* 1ms/1ms reservation */
  attr.sched_policy = SCHED_DEADLINE;
  attr.sched_runtime = attr.sched_deadline = attr.sched_period = 1000000; // nanoseconds

  /* Initialize callback (non real-time yet) */
  void * data = init(argc, argv);

  /* Set scheduler policy */
  if(sched_setattr(0, &attr, 0) < 0)
  {
    printf("sched_setattr failed: %m\n");
    return -2;
  }

  /* Run */
  run(data);

  return 0;
}
