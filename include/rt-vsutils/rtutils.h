/**
 * \file rtutils.h
 * \brief Utils functions for real-time.
 * \author Sebastien Vincent
 * \date 2017
 */

#ifndef RTVSUTILS_RTUTILS_H
#define RTVSUTILS_RTUTILS_H

#include <unistd.h>
#include <pthread.h>

/**
 * \enum cpufreq_governor
 * \param Enumeration of different CPU frequency change governor.
 */
enum cpufreq_governor
{
    /**
     * \brief Other mode.
     */
    CPUFREQ_OTHER = -1,

    /**
     * \brief Powersave mode.
     *
     * CPUs runs at lowest speed all the time.
     */
    CPUFREQ_POWERSAVE,

    /**
     * \brief Performance mode.
     *
     * CPUs runs at full speed all the time.
     */
    CPUFREQ_PERFORMANCE,

    /**
     * \brief On demand mode.
     *
     * Changes occurs when load increases/decreases.
     */
    CPUFREQ_ONDEMAND,

    /**
     * \brief Conservative mode.
     *
     * Similar as CPUFREQ_ONDEMAND but less change.
     */
    CPUFREQ_CONSERVATIVE,
};

/**
 * \struct rt_prio
 * \brief Real-time priority information.
 */
struct rt_prio
{
    /**
     * \brief Real-time priority.
     *
     * It can be either SCHED_FIFO, SCHED_RR or SCHED_OTHER.
     */
    int policy;

    /**
     * \brief Real-time priority.
     */
    unsigned int priority;
};

/**
 * \brief Lock and reserve memory for stack.
 *
 * It is generally used in the beginning of the program to avoid
 * non-deterministic time during fault-page for allocation.
 * \param stack_size Size of the stack.
 * \return 0 if success, negative value otherwise.
 */
int mem_lock_reserve(size_t stack_size);

/**
 * \brief Disable the percent of time reserved for shared-time processes on 
 * GNU/Linux systems.
 * \return 0 if success, negative value otherwise.
 */
int rt_disable_watchdog(void);

/**
 * \brief Returns the CPU identifier of the current process at the time of call.
 * \return ID of CPU, negative value otherwise.
 */
int process_get_current_cpu(void);

/**
 * \brief Returns the CPU identifier of the current thread at the time of call.
 * \return ID of CPU, negative value otherwise.
 */
int thread_get_current_cpu(void);

/**
 * \brief Sets the affinity of a process on the CPU indexes define in array.
 * \param pid PID of the process.
 * \param cpus array of CPU index (first CPU is 0, second is 1, ...).
 * \param cpus_size size of the array.
 * \return 0 if success, negative value otherwise.
 */
int process_set_affinity(pid_t pid, int* cpus, size_t cpus_size);

/**
 * \brief Returns the affinity of a process on the CPU indexes define in array.
 * \param pid PID of the process.
 * \param cpus array of CPU index (first CPU is 0, second is 1, ...).
 * \param cpus_size size of the array.
 * \return 0 if success, negative value otherwise.
 */
int process_get_affinity(pid_t pid, int* cpus, size_t cpus_size);

/**
 * \brief Sets the affinity of a thread on the CPU indexes define in array.
 * \param th ID of the thread.
 * \param cpus array of CPU index (first CPU is 0, second is 1, ...).
 * \param cpus_size size of the array.
 * \return 0 if success, negative value otherwise.
 */
int thread_set_affinity(pthread_t th, int* cpus, size_t cpus_size);

/**
 * \brief Returns the affinity of a thread on the CPU indexes define in array.
 * \param th ID of the thread.
 * \param cpus array of CPU index (first CPU is 0, second is 1, ...).
 * \param cpus_size size of the array.
 * \return 0 if success, negative value otherwise.
 */
int thread_get_affinity(pthread_t th, int* cpus, size_t cpus_size);

/**
 * \brief Sets shared-time priority of a process.
 * \param pid PID of the process to change priority.
 * \param priority priority to set (range from -20 to 19).
 * \return 0 if success, negative value otherwise.
 */
int process_set_priority(pid_t pid, int priority);

/**
 * \brief Returns shared-time priority of a process.
 * \param pid PID of the process to change priority.
 * \return priority of the process.
 * \note if returns -1, check if errno is set to seee if it is an error or not.
 */
int process_get_priority(pid_t pid);

/**
 * \brief Sets shared-time priority of a thread.
 * \param th ID of the thread to change priority.
 * \param priority priority to set (range from -20 to 19).
 * \return 0 if success, negative value otherwise.
 */
int thread_set_priority(pthread_t th, int priority);

/**
 * \brief Returns shared-time priority of a thread.
 * \param th ID of the thread to change priority.
 * \return priority of the thread.
 * \note if returns -1, check if errno is set to seee if it is an error or not.
 */
int thread_get_priority(pthread_t th);

/**
 * \brief Sets real-time priority of a process.
 * \param pid PID of the process to change priority.
 * \param priority real-time priority information.
 * \return 0 if success, negative value otherwise.
 */
int process_set_rt_priority(pid_t pid, struct rt_prio* priority);

/**
 * \brief Returns real-time priority of a process.
 * \param pid PID of the process to change priority.
 * \param priority real-time priority information.
 * \return 0 if success, negative value otherwise.
 */
int process_get_rt_priority(pid_t pid, struct rt_prio* priority);

/**
 * \brief Sets real-time priority of a thread.
 * \param th identifier of the thread to change priority.
 * \param priority real-time priority information.
 * \return 0 if success, negative value otherwise.
 */
int thread_set_rt_priority(pthread_t th, struct rt_prio* priority);

/**
 * \brief Returns real-time priority of a thread.
 * \param th identifier of the thread to change priority.
 * \param priority real-time priority information.
 * \return 0 if success, negative value otherwise.
 */
int thread_get_rt_priority(pthread_t th, struct rt_prio* priority);

/**
 * \brief Change CPU frequency for all CPUs.
 * \param mode governor to use.
 * \return 0 if success, negative value otherwise.
 */
int cpufreq_set_governor_all(enum cpufreq_governor mode);

/**
 * \brief Change CPU frequency for a specific CPU.
 * \param mode governor to use.
 * \param cpu CPU id to apply the governor.
 * \return 0 if success, negative value otherwise.
 */
int cpufreq_set_governor(enum cpufreq_governor mode, unsigned int cpu);

/**
 * \brief Returns CPU frequency mode.
 * \param cpu CPU id to apply the governor.
 * \return governor used.
 */
enum cpufreq_governor cpufreq_get_governor(unsigned int cpu);

#endif /* RTVSUTILS_RTUTILS_H */

