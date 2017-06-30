/**
 * \file test_priority.
 * \brief Tests for process/thread priority.
 * \author Sebastien Vincent
 * \date 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "rtutils.h"

#define RT_TEST 1

/**
 * \brief Thread function.
 * \param data not used.
 * \return NULL.
 */
static void* thread_function(void* data)
{
  int ret = 0;

  (void)data;

  ret = thread_get_priority(pthread_self());
  if(ret == -1 && errno != 0)
  {
    perror("thread_get_priority");
  }
  fprintf(stdout, "Thread priority: %d\n", ret);

  ret = thread_set_priority(pthread_self(), 15);
  if(ret != 0)
  {
    perror("thread_set_priority");
  }
  
#if RT_TEST
  {
    struct rt_prio prio;
    printf("RT priority\n");
    prio.priority = 50;
    prio.policy = SCHED_FIFO;
    ret = thread_set_rt_priority(pthread_self(), &prio);
  }
#endif

  ret = thread_get_priority(pthread_self());
  if(ret == -1 && errno != 0)
  {
    perror("thread_get_priority");
  }
  else
  {
    fprintf(stdout, "Thread priority: %d\n", ret);
  }

  ret = thread_set_priority(pthread_self(), -15);
  if(ret != 0)
  {
    perror("thread_set_priority");
  }
  else
  {
    ret = thread_get_priority(pthread_self());

    if(ret == -1 && errno != 0)
    {
      perror("thread_get_priority");
    }
    else
    {
      fprintf(stdout, "Thread priority: %d\n", ret);
    }
  }
  
  return NULL;
}

/**
 * \brief Main entry point.
 * \param argc number of arguments.
 * \param argv array of arguments.
 * \return EXIT_SUCCESS if success, EXIT_FAILURE otherwise.
 */
int main(int argc, char** argv)
{
  int ret = 0;
  pthread_t th;
  pthread_attr_t th_attr;

  (void)argc;
  (void)argv;
 
  pthread_attr_init(&th_attr);
  /* pthread_attr_setinheritsched(&th_attr, PTHREAD_EXPLICIT_SCHED); */
  
  /* thread tests */
  if(pthread_create(&th, &th_attr, thread_function, NULL) != 0)
  {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  pthread_join(th, NULL);
  fprintf(stdout, "\n");

  /* process tests */
  ret = process_get_priority(getpid());
  if(ret == -1 && errno != 0)
  {
    perror("process_get_priority");
  }
  fprintf(stdout, "Process priority: %d\n", ret);

  ret = process_set_priority(getpid(), 15);
  if(ret != 0)
  {
    perror("process_set_priority");
  }

#if RT_TEST
  {
    struct rt_prio prio;
    printf("RT priority\n");
    prio.priority = 50;
    prio.policy = SCHED_FIFO;
    ret = process_set_rt_priority(getpid(), &prio);
  }
#endif

  ret = process_get_priority(getpid());
  if(ret == -1 && errno != 0)
  {
    perror("process_get_priority");
  }
  else
  {
    fprintf(stdout, "Process priority: %d\n", ret);
  }

  ret = process_set_priority(getpid(), -15);
  if(ret != 0)
  {
    perror("process_set_priority");
  }
  else
  {
    ret = process_get_priority(getpid());

    if(ret == -1 && errno != 0)
    {
      perror("process_get_priority");
    }
    else
    {
      fprintf(stdout, "Process priority: %d\n", ret);
    }
  }

  return EXIT_SUCCESS;
}

