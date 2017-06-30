/**
 * \file test_memlock.
 * \brief Tests for memlock.
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

  if(mem_lock_reserve(65536) != 0)
  {
    perror("mem_lock_reserve failure");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "mem_lock_reserve success\n");

  return EXIT_SUCCESS;
}

