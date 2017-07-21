/**
 * \file test_periodic_task.
 * \brief Tests for thread periodic task.
 * \author Sebastien Vincent
 * \date 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "rtutils.h"

/**
 * \brief Task to execute periodically.
 * \param data data for the task.
 */
void th_task(void* data)
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);

    fprintf(stdout, "%ld.%ld: %p\n", time.tv_sec, time.tv_nsec, data);
}

/**
 * \brief Dedicated thread to execute a periodic task.
 * \param data data for the task.
 * \return NULL.
 */
void* th_periodic(void* data)
{
    if(thread_periodic_task(th_task, data, 1000000000) != 0)
    {
        fprintf(stderr, "Failed to launch periodic task\n");
        return NULL;
    }

    return NULL;
}

/**
 * \brief Entry point of the program.
 * \param argc number of arguments.
 * \param argv array of arguments.
 * \return EXIT_SUCCESS or EXIT_FAILURE.
 */
int main(int argc, char** argv)
{
    pthread_t th;

    (void)argc;
    (void)argv;

    if(pthread_create(&th, NULL, th_periodic, NULL) != 0)
    {
        fprintf(stderr, "Failed to launch thread\n");
        exit(EXIT_FAILURE);
    }

    sleep(5);
    /* stop the periodic thread */
    pthread_cancel(th);

    pthread_join(th, NULL);
    return EXIT_SUCCESS;
}

