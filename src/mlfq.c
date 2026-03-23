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

void pq_init(ProcessQueue *pq, int capacity)
{
    pq->processes = malloc(capacity * sizeof(Process *));
    pq->size = 0;
    pq->capacity = capacity;
}

void pq_push(ProcessQueue *pq, Process *process)
{
    if (pq->size == pq->capacity)
    {
        pq->capacity *= 2;
        pq->processes = realloc(pq->processes, pq->capacity * sizeof(Process *));
    }
    pq->processes[pq->size++] = process;
}

Process *pq_pop(ProcessQueue *pq)
{
    if (pq->size > 0)
    {
        Process *proc = pq->processes[0];
        memmove(pq->processes, pq->processes + 1, (pq->size - 1) * sizeof(Process *));
        pq->size--;
        return proc;
    }
    return NULL;
}

void pq_free(ProcessQueue *pq)
{
    free(pq->processes);
    pq->processes = NULL;
    pq->size = 0;
    pq->capacity = 0;
}

int schedule_mlfq(SchedulerState *state, MLFQScheduler *mlfq_config)
{
    // Implementation for MLFQ scheduling
    int time = 0;
    int completed = 0;
    int num_processes = state->num_processes;
    int num_queues = mlfq_config->num_queues;
    char last_pid[16] = "";

    // Initialize queues
    ProcessQueue *queues = malloc(num_queues * sizeof(ProcessQueue));
    for (int i = 0; i < num_queues; i++)
    {
        pq_init(&queues[i], num_processes + 1);
    }

    int *admitted_processes = calloc(num_processes, sizeof(int)); // Track admitted processes
    mlfq_config->last_boost = 0;

    while (completed < num_processes)
    {

        // 1. Admit new processes to the highest priority queue
        for (int i = 0; i < num_processes; i++)
        {
            if (!admitted_processes[i] && state->processes[i].arrival_time <= time)
            {
                Process *proc = &state->processes[i];
                proc->priority = 0; // Start at highest priority
                pq_push(&queues[0], proc);
                admitted_processes[i] = 1;
            }
        }

        // 2. Check for priority boost
        if (mlfq_config->boost_period > 0 && time - mlfq_config->last_boost >= mlfq_config->boost_period)
        {
            for (int i = 1; i < num_queues; i++)
            {
                while (queues[i].size > 0)
                {
                    Process *proc = pq_pop(&queues[i]);
                    proc->priority = 0;      // Boost to highest priority
                    proc->time_in_queue = 0; // Reset time in queue after boost
                    pq_push(&queues[0], proc);
                }
            }
            mlfq_config->last_boost = time;
        }

        // 3. Find the highest priority non-empty queue
        int queue_index = -1;
        for (int i = 0; i < num_queues; i++)
        {
            if (queues[i].size > 0)
            {
                queue_index = i;
                break;
            }
        }

        if (queue_index == -1)
        {
            int next_arrival = -1;
            for (int i = 0; i < num_processes; i++)
            {
                if (!admitted_processes[i] && (next_arrival == -1 || state->processes[i].arrival_time < next_arrival))
                {
                    next_arrival = state->processes[i].arrival_time;
                }
            }
            if (next_arrival == -1)
            {
                break; // No more processes to admit
            }

            time = next_arrival; // Move time forward to next arrival
            continue;            // No processes ready, move time forward to next arrival
        }

        MLFQQueue *mq = &mlfq_config->queues[queue_index];
        Process *proc = pq_pop(&queues[queue_index]); // Get the first process in the queue

        if (proc->start_time == -1)
        {
            proc->start_time = time; // First time the process is executed
        }

        if (last_pid[0] != '\0' && strcmp(last_pid, proc->pid) != 0)
        {
            state->context_switches++; // Increment context switch count if switching to a different process
        }
        strncpy(last_pid, proc->pid, 15);
        last_pid[15] = '\0';

        // 4. Run the process for the appropriate time slice
        int time_slice = (mq->time_quantum > 0) ? mq->time_quantum : proc->remaining_time;
        int allotment_time = (mq->allotment > 0) ? mq->allotment - proc->time_in_queue : time_slice;
        int run_time = (time_slice < allotment_time) ? time_slice : allotment_time;

        if (run_time > proc->remaining_time)
        {
            run_time = proc->remaining_time;
        }

        int check_time = time + run_time;
        for (int i = 0; i < num_processes; i++)
        {
            if (!admitted_processes[i] &&
                state->processes[i].arrival_time > time &&
                state->processes[i].arrival_time <= check_time)
            {
                check_time = state->processes[i].arrival_time;
            }
        }

        if (mlfq_config->boost_period > 0)
        {
            int next_boost_time = mlfq_config->last_boost + mlfq_config->boost_period;
            if (next_boost_time > time && next_boost_time < check_time)
            {
                check_time = next_boost_time;
            }
        }

        run_time = check_time - time;
        proc->remaining_time -= run_time;
        proc->time_in_queue += run_time;
        time += run_time;

        // 5. Check if the process has finished
        if (proc->remaining_time == 0)
        {
            proc->finish_time = time;
            completed++;
        }
        else
        {
            if (mq->allotment > 0 && proc->time_in_queue >= mq->allotment)
            {
                // Reset time in queue after demotion
                if (proc->priority < num_queues - 1)
                {
                    proc->priority++;
                    proc->time_in_queue = 0;
                }
            }
            pq_push(&queues[proc->priority], proc); // Re-add to appropriate queue
        }
    }

    // 6. Cleanup
    for (int i = 0; i < num_queues; i++)
    {
        pq_free(&queues[i]);
    }zz
    free(queues);
    free(admitted_processes);

    return 0;
}