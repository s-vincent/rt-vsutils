/**
 * \file test_affinity.
 * \brief Tests for process/thread affinity.
 * \author Sebastien Vincent
 * \date 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rtutils.h"

static void* thread_function(void* data)
{
  int ret = 0;
  int cpus[1024];

  (void)data;

  fprintf(stdout, "Thread CPU %d\n", thread_get_current_cpu());

  ret = thread_get_affinity(pthread_self(), cpus, sizeof(cpus));
  if(ret <= 0)
  {
    perror("thread_get_affinity");
    return NULL;
  }

  fprintf(stdout, "Affinity for thread: ");
  for(int i = 0 ; i < ret ; i++)
  {
    fprintf(stdout, "%d ", cpus[i]);

    if(i > 0)
    {
      cpus[i] = -1;
    }
  }
  fprintf(stdout, "\n");

  fprintf(stdout, "Set affinity\n");
  ret = thread_set_affinity(pthread_self(), cpus, ret);
  if(ret != 0)
  {
    fprintf(stdout, "Failed to set affinity\n");
    return NULL;
  }

  fprintf(stdout, "Affinity for thread: ");
  ret = thread_get_affinity(pthread_self(), cpus, sizeof(cpus));
  if(ret <= 0)
  {
    perror("thread_get_affinity");
    return NULL;
  }

  for(int i = 0 ; i < ret ; i++)
  {
    fprintf(stdout, "%d ", cpus[i]);
  }
  fprintf(stdout, "\n");

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
  int cpus[1024];
  int ret = 0;
  pthread_t th;

  (void)argc;
  (void)argv;
  
  /* thread tests */
  if(pthread_create(&th, NULL, thread_function, NULL) != 0)
  {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  pthread_join(th, NULL);
  fprintf(stdout, "\n");

  /* process tests */
  fprintf(stdout, "Process CPU %d\n", process_get_current_cpu());

  ret = process_get_affinity(getpid(), cpus, sizeof(cpus));
  if(ret <= 0)
  {
    perror("process_get_affinity");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "Affinity for process: ");
  for(int i = 0 ; i < ret ; i++)
  {
    fprintf(stdout, "%d ", cpus[i]);

    if(i > 0)
    {
      cpus[i] = -1;
    }
  }
  fprintf(stdout, "\n");

  fprintf(stdout, "Set affinity\n");
  ret = process_set_affinity(getpid(), cpus, ret);
  if(ret != 0)
  {
    fprintf(stdout, "Failed to set affinity\n");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "Affinity for process: ");
  ret = process_get_affinity(getpid(), cpus, sizeof(cpus));
  if(ret <= 0)
  {
    perror("process_get_affinity");
    exit(EXIT_FAILURE);
  }

  for(int i = 0 ; i < ret ; i++)
  {
    fprintf(stdout, "%d ", cpus[i]);
  }
  fprintf(stdout, "\n");

  return EXIT_SUCCESS;
}

