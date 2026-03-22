/*
 * file: mlfq.c
 * Multi-level feedback queue scheduling
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/gantt.h"
#include "../include/scheduler.h"
#include "../include/metrics.h"

int schedule_mlfq(SchedulerState *state, MLFQScheduler *mlfq_config)
{
    // Implementation for MLFQ scheduling
    int time = 0;
    int completed = 0;
    int num_processes = state->num_processes;

    for (int i = 0; i < num_processes; i++)
    {
        state->processes[i].remaining_time = state->processes[i].burst_time;
        state->processes[i].priority = 0; // Start at highest priority
        state->processes[i].time_in_queue = 0;
    }

    while (completed < num_processes)
    {
        // Check for priority boost
        if (mlfq_config->boost_period > 0 && time - mlfq_config->last_boost >= mlfq_config->boost_period)
        {
            for (int i = 0; i < num_processes; i++)
            {
                if (state->processes[i].remaining_time > 0)
                {
                    state->processes[i].priority = 0; // Boost to highest priority
                    state->processes[i].time_in_queue = 0;
                }
            }
            mlfq_config->last_boost = time;
        }

        // Find the highest priority non-empty queue
        int queue_index = -1;
        for (int i = 0; i < mlfq_config->num_queues; i++)
        {
            if (mlfq_config->queues[i].size > 0)
            {
                queue_index = i;
                break;
            }
        }

        if (queue_index == -1)
        {
            time++; // No processes ready, advance time
            continue;
        }

        MLFQQueue *queue = &mlfq_config->queues[queue_index];
        Process *proc = &queue->queue[0]; // Get the first process in the queue

        // Execute the process for its time quantum or until completion
        int exec_time = (queue->time_quantum > 0) ? queue->time_quantum : proc->remaining_time;
        exec_time = (exec_time < proc->remaining_time) ? exec_time : proc->remaining_time;

        proc->remaining_time -= exec_time;
        time += exec_time;

        if (proc->remaining_time == 0)
        {
            completed++;
            // Remove from queue and update metrics
        }
        else
        {
            proc->time_in_queue += exec_time;
            if (proc->time_in_queue >= queue->allotment && queue_index < mlfq_config->num_queues - 1)
            {
                proc->priority++;
                proc->time_in_queue = 0;
                // Move to next lower priority queue
            }
        }
    }

    return 0;
}