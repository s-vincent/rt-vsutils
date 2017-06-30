/**
 * \file test_rt_watchdog.
 * \brief Tests for RT watchdog.
 * \author Sebastien Vincent
 * \date 2017
 */

#include <stdio.h>
#include <stdlib.h>

#include "rtutils.h"

/**
 * \brief Main entry point.
 * \param argc number of arguments.
 * \param argv array of arguments.
 * \return EXIT_SUCCESS if success, EXIT_FAILURE otherwise.
 */
int main(int argc, char** argv)
{
  (void)argc;
  (void)argv;

  if(rt_disable_watchdog() != 0)
  {
    perror("rt_disable_watchdog failure");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "rt_disable_watchdog success\n");

  return EXIT_SUCCESS;
}

