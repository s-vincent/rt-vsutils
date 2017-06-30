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

/**
 * \brief Returns string name of the scheduling policies.
 * \param policy scheduling policy.
 * \return string name.
 */
static const char* get_policy(int policy)
{
  static const char* const POLICIES[] = { "SCHED_FIFO",
    "SCHED_RR", "SCHED_OTHER", "ERROR"};

  switch(policy)
  {
  case SCHED_FIFO:
    return POLICIES[0];
    break;
  case SCHED_RR:
    return POLICIES[1];
    break;
  case SCHED_OTHER:
    return POLICIES[2];
    break;
  default:
    return POLICIES[3];
  }

  return NULL;
}

/**
 * \brief Thread function.
 * \param data not used.
 * \return NULL.
 */
static void* thread_function(void* data)
{
  int ret = 0;
  struct rt_prio prio;

  (void)data;

  /* thread tests */
  ret = thread_get_rt_priority(pthread_self(), &prio);
  if(ret != 0)
  {
    perror("thread_get_rt_priority");
  }
  fprintf(stdout, "Thread RT priority: %s %d\n", get_policy(prio.policy), 
      prio.priority);

  prio.priority = 50;
  prio.policy = SCHED_FIFO;
  ret = thread_set_rt_priority(pthread_self(), &prio);
  if(ret != 0)
  {
    perror("thread_set_priority");
  }
  
  ret = thread_get_rt_priority(pthread_self(), &prio);
  fprintf(stdout, "Thread RT priority: %s %d\n", get_policy(prio.policy), 
      prio.priority);

  prio.priority = 80;
  prio.policy = SCHED_RR;
  ret = thread_set_rt_priority(pthread_self(), &prio);
  if(ret != 0)
  {
    perror("thread_set_rt_priority");
  }
  
  ret = thread_get_rt_priority(pthread_self(), &prio);
  fprintf(stdout, "Thread RT priority: %s %d\n", get_policy(prio.policy), 
      prio.priority);

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
  struct rt_prio prio;
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
  ret = process_get_rt_priority(getpid(), &prio);
  if(ret != 0)
  {
    perror("process_get_rt_priority");
  }
  fprintf(stdout, "Process RT priority: %s %d\n", get_policy(prio.policy), 
      prio.priority);

  prio.priority = 50;
  prio.policy = SCHED_FIFO;
  ret = process_set_rt_priority(getpid(), &prio);
  if(ret != 0)
  {
    perror("process_set_priority");
  }
  
  ret = process_get_rt_priority(getpid(), &prio);
  fprintf(stdout, "Process RT priority: %s %d\n", get_policy(prio.policy), 
      prio.priority);

  prio.priority = 80;
  prio.policy = SCHED_RR;
  ret = process_set_rt_priority(getpid(), &prio);
  if(ret != 0)
  {
    perror("process_set_rt_priority");
  }
  
  ret = process_get_rt_priority(getpid(), &prio);
  fprintf(stdout, "Process RT priority: %s %d\n", get_policy(prio.policy), 
      prio.priority);

  return EXIT_SUCCESS;
}

