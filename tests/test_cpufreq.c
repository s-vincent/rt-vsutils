/**
 * \file test_priority.
 * \brief Tests for process/thread priority.
 * \author Sebastien Vincent
 * \date 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rtutils.h"

/**
 * \brief Main entry point.
 * \param argc number of arguments.
 * \param argv array of arguments.
 * \return EXIT_SUCCESS if success, EXIT_FAILURE otherwise.
 */
int main(int argc, char** argv)
{
  enum cpufreq_governor governor = cpufreq_get_governor(0);

  (void)argc;
  (void)argv;

  fprintf(stdout, "CPU frequency governor: %d\n", governor);

  if(cpufreq_set_governor(CPUFREQ_PERFORMANCE, 0) != 0)
  {
    perror("cpufreq_set_governor performance");
  }
  sleep(5);
  
  if(cpufreq_set_governor(CPUFREQ_POWERSAVE, 0) != 0)
  {
    perror("cpufreq_set_governor powersave");
  }
  sleep(5);

  if(cpufreq_set_governor(CPUFREQ_ONDEMAND, 0) != 0)
  {
    perror("cpufreq_set_governor ondemand");
  }
  sleep(5);

  if(cpufreq_set_governor(CPUFREQ_CONSERVATIVE, 0) != 0)
  {
    perror("cpufreq_set_governor conservative");
  }
  sleep(5);

  if(governor != -1)
  {
    if(cpufreq_set_governor(governor, 0) != 0)
    {
      perror("cpufreq_set_governor");
      exit(EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}

