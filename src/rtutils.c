/**
 * \file rtutils.c
 * \brief Utils functions for real-time.
 * \author Sebastien Vincent
 * \date 2017
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <sched.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <syscall.h>

#include "rtutils.h"

/**
 * \brief Path to configure the governor via /sys.
 */
static const char* const CPUFREQ_GOVERNOR_PATH = 
  "/sys/devices/system/cpu/cpu%u/cpufreq/scaling_governor";

int mem_lock_reserve(size_t stack_size)
{
  char stack[stack_size];

  memset(stack, 0x00, sizeof(stack));

  return mlockall(MCL_CURRENT | MCL_FUTURE);
}

int rt_disable_watchdog(void)
{
  int fd = open("/proc/sys/kernel/sched_rt_runtime_us", O_WRONLY);
  const char* const disable_value = "-1";

  if(fd == -1)
  {
    return -1;
  }

  if(write(fd, disable_value, strlen(disable_value)) == -1)
  {
    return -1;
  }

  close(fd);
  return 0;
}

int process_get_current_cpu()
{
  return sched_getcpu();
}

int thread_get_current_cpu()
{
  return sched_getcpu();
}

int process_set_affinity(pid_t pid, int* cpus, size_t cpus_size)
{
  cpu_set_t mask;

  CPU_ZERO(&mask);

  for(size_t i = 0 ; i < cpus_size ; i++)
  {
    if(cpus[i] >= 0)
    {
      CPU_SET(cpus[i], &mask);
    }
  }

  return sched_setaffinity(pid, sizeof(cpu_set_t), &mask);
}

int process_get_affinity(pid_t pid, int* cpus, size_t cpus_size)
{
  cpu_set_t mask;
  long nb_cpus = sysconf(_SC_NPROCESSORS_ONLN);
  int nb = 0;

  if(cpus && cpus_size < (size_t)nb_cpus)
  {
    return -EINVAL;
  }

  CPU_ZERO(&mask);

  if(sched_getaffinity(pid, sizeof(cpu_set_t), &mask) != 0)
  {
    return -1;
  }

  for(long i = 0 ; i < nb_cpus ; i++)
  {
    if(CPU_ISSET(i, &mask))
    {
      if(cpus)
      {
        cpus[nb] = i;
      }
      nb++;
    }
  }

  return nb;
}

int thread_set_affinity(pthread_t th, int* cpus, size_t cpus_size)
{
  cpu_set_t mask;

  CPU_ZERO(&mask);

  for(size_t i = 0 ; i < cpus_size ; i++)
  {
    CPU_SET(cpus[i], &mask);
  }

  return pthread_setaffinity_np(th, sizeof(cpu_set_t), &mask);
}

int thread_get_affinity(pthread_t th, int* cpus, size_t cpus_size)
{
  cpu_set_t mask;
  long nb_cpus = sysconf(_SC_NPROCESSORS_ONLN);
  int nb = 0;

  if(cpus && cpus_size < (size_t)nb_cpus)
  {
    return -EINVAL;
  }

  CPU_ZERO(&mask);

  if(pthread_getaffinity_np(th, sizeof(cpu_set_t), &mask) != 0)
  {
    return -1;
  }

  for(long i = 0 ; i < nb_cpus ; i++)
  {
    if(CPU_ISSET(i, &mask))
    {
      if(cpus)
      {
        cpus[nb] = i;
      }
      nb++;
    }
  }

  return nb;
}

int process_set_priority(pid_t pid, int priority)
{
  if(priority < -20 || priority > 19)
  {
    errno = EINVAL;
    return -1;
  }

  return setpriority(PRIO_PROCESS, pid, priority);
}

int process_get_priority(pid_t pid)
{
  errno = 0;
  return getpriority(PRIO_PROCESS, pid);
}

int thread_set_priority(pthread_t th, int priority)
{
  pid_t tid = syscall(SYS_gettid);

  (void)th;

  /* WARNING non-portable: on Linux, the nice() priority is per-thread */
  return setpriority(PRIO_PROCESS, tid, priority);
}

int thread_get_priority(pthread_t th)
{
  pid_t tid = syscall(SYS_gettid);

  (void)th;

  /* WARNING non-portable: on Linux, the nice() priority is per-thread */
  return getpriority(PRIO_PROCESS, tid);
}

int process_set_rt_priority(pid_t pid, struct rt_prio* priority)
{
  struct sched_param param;

  if(!priority)
  {
    errno = EINVAL;
    return -1;
  }

  memset(&param, 0x00, sizeof(struct sched_param));
  param.sched_priority = priority->priority;

  return sched_setscheduler(pid, priority->policy, &param);
}

int process_get_rt_priority(pid_t pid, struct rt_prio* priority)
{
  struct sched_param param;
  int ret = 0;

  if(!priority)
  {
    errno = EINVAL;
    return -1;
  }

  ret = sched_getparam(pid, &param);
  if(ret != 0)
  {
    return ret;
  }

  ret = sched_getscheduler(pid);
  if(ret < 0)
  {
    return ret;
  }

  priority->priority = param.sched_priority;
  priority->policy = ret; 

  return 0;
}

int thread_set_rt_priority(pthread_t th, struct rt_prio* priority)
{
  struct sched_param param;
  int ret = 0;

  if(!priority)
  {
    errno = EINVAL;
    return -1;
  }

  memset(&param, 0x00, sizeof(struct sched_param));
  param.sched_priority = priority->priority;

  ret = pthread_setschedparam(th, priority->policy, &param);
  if(ret != 0)
  {
    return ret;
  }

  return 0;
}

int thread_get_rt_priority(pthread_t th, struct rt_prio* priority)
{
  struct sched_param param;
  int policy = 0;
  int ret = 0;

  if(!priority)
  {
    errno = EINVAL;
    return -1;
  }

  ret = pthread_getschedparam(th, &policy, &param);
  if(ret != 0)
  {
    return ret;
  }

  priority->priority = param.sched_priority;
  priority->policy = policy;

  return 0;
}

int cpufreq_set_governor_all(enum cpufreq_governor mode)
{
  long nb_cpus = sysconf(_SC_NPROCESSORS_ONLN);

  for(long i = 0 ; i < nb_cpus ; i++)
  {
    if(cpufreq_set_governor(mode, i) == -1)
    {
      return -1;
    }
  }

  return 0;
}

int cpufreq_set_governor(enum cpufreq_governor mode, unsigned int cpu)
{
  long nb_cpus = sysconf(_SC_NPROCESSORS_ONLN);
  char path[256];
  const char* governor = NULL;
  int ret = 0;
  int fd = -1;

  if(cpu >= nb_cpus)
  {
    return -1;
  }

  snprintf(path, sizeof(path), CPUFREQ_GOVERNOR_PATH, cpu);
  path[sizeof(path) - 1] = 0x00;

  fd = open(path, O_WRONLY);

  if(fd == -1)
  {
    return -1;
  }

  switch(mode)
  {
    case CPUFREQ_POWERSAVE:
      governor = "powersave";
      break;
    case CPUFREQ_PERFORMANCE:
      governor = "performance";
      break;
    case CPUFREQ_ONDEMAND:
      governor = "ondemand";
      break;
    case CPUFREQ_CONSERVATIVE:
      governor = "conservative";
      break;
    default:
      governor = NULL;
      break;
  }

  if(!governor)
  {
    return -1;
  }

  ret = write(fd, governor, strlen(governor));
  close(fd);

  if(ret == -1)
  {
    return -1;
  }

  return 0;
}

enum cpufreq_governor cpufreq_get_governor(unsigned int cpu)
{
  long nb_cpus = sysconf(_SC_NPROCESSORS_ONLN);
  char path[256];
  char value[256];
  int ret = 0;
  int fd = -1;

  if(cpu >= nb_cpus)
  {
    return -1;
  }

  snprintf(path, sizeof(path), CPUFREQ_GOVERNOR_PATH, cpu);
  path[sizeof(path) - 1] = 0x00;

  fd = open(path, O_WRONLY);

  if(fd == -1)
  {
    return CPUFREQ_OTHER;
  }

  ret = read(fd, value, sizeof(value));
  value[sizeof(value) - 1] = 0x00;
  close(fd);

  if(ret == -1)
  {
    return CPUFREQ_OTHER;
  }

  if(!strcmp(value, "powersave"))
  {
    return CPUFREQ_POWERSAVE;
  }
  else if(!strcmp(value, "performance"))
  {
    return CPUFREQ_PERFORMANCE;
  }
  else if(!strcmp(value, "ondemand")) 
  {
    return CPUFREQ_ONDEMAND;
  }
  else if(!strcmp(value, "conservative"))
  {
    return CPUFREQ_CONSERVATIVE;
  }
  else
  {
    return CPUFREQ_OTHER;
  }
}

