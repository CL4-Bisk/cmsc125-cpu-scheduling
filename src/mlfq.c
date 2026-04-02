/*
 * file: mlfq.c
 * Multi-level feedback queue scheduling
 */
#include <stdlib.h>
#include "../include/gantt.h"
#include "../include/scheduler.h"
#include "../include/metrics.h"
#include "../include/sched_utils.h"

int schedule_mlfq(SchedulerState *state, MLFQScheduler *mlfq_config)
{
    // Implementation for MLFQ scheduling
    SchedulerContext ctx;
    ctx_init(&ctx, state);
    int num_queues = mlfq_config->num_queues;

    // Initialize queues
    ProcessQueue *queues = malloc(num_queues * sizeof(ProcessQueue));
    for (int i = 0; i < num_queues; i++)
    {
        pq_init(&queues[i], ctx.num_processes + 1);
    }

    int *admitted_processes = calloc(ctx.num_processes, sizeof(int)); // Track admitted processes
    mlfq_config->last_boost = 0;

    while (ctx.completed < ctx.num_processes)
    {

        // 1. Admit new processes to the highest priority queue
        for (int i = 0; i < ctx.num_processes; i++)
        {
            if (!admitted_processes[i] && state->processes[i].arrival_time <= ctx.time)
            {
                Process *proc = &state->processes[i];
                proc->priority = 0; // Start at highest priority
                pq_push(&queues[0], proc);
                admitted_processes[i] = 1;
            }
        }

        // 2. Check for priority boost
        if (mlfq_config->boost_period > 0 && ctx.time - mlfq_config->last_boost >= mlfq_config->boost_period)
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
            mlfq_config->last_boost = ctx.time;
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
            int next_arrival = ctx_next_arrival(state, &ctx, admitted_processes);
            if (next_arrival == -1)
            {
                break; // No more processes to admit
            }

            ctx.time = next_arrival; // Move time forward to next arrival
            continue;                // No processes ready, move time forward to next arrival
        }

        MLFQQueue *mq = &mlfq_config->queues[queue_index];
        Process *proc = pq_pop(&queues[queue_index]); // Get the first process in the queue

        if (proc->start_time == -1)
        {
            log_process_start(ctx.time, proc->pid);
            proc->start_time = ctx.time; // First time the process is executed
        }

        ctx_track_switch(state, &ctx, proc->pid);

        // 4. Run the process for the appropriate time slice
        int time_slice = (mq->time_quantum > 0) ? mq->time_quantum : proc->remaining_time;
        int allotment_time = (mq->allotment > 0) ? mq->allotment - proc->time_in_queue : time_slice;
        int run_time = (time_slice < allotment_time) ? time_slice : allotment_time;

        if (run_time > proc->remaining_time)
        {
            run_time = proc->remaining_time;
        }

        int check_time = ctx.time + run_time;
        for (int i = 0; i < ctx.num_processes; i++)
        {
            if (!admitted_processes[i] &&
                state->processes[i].arrival_time > ctx.time &&
                state->processes[i].arrival_time <= check_time)
            {
                check_time = state->processes[i].arrival_time;
            }
        }

        if (mlfq_config->boost_period > 0)
        {
            int next_boost_time = mlfq_config->last_boost + mlfq_config->boost_period;
            if (next_boost_time > ctx.time && next_boost_time < check_time)
            {
                check_time = next_boost_time;
            }
        }

        run_time = check_time - ctx.time;
        proc->remaining_time -= run_time;
        proc->time_in_queue += run_time;
        int run_start = ctx.time;
        ctx.time += run_time;
        gantt_extend(state, proc->pid, run_start, ctx.time);

        // 5. Check if the process has finished
        if (proc->remaining_time == 0)
        {
            proc->finish_time = ctx.time;
            log_process_finish(ctx.time, proc->pid);
            ctx.completed++;
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
    finish_scheduler(state);
    // 6. Cleanup
    for (int i = 0; i < num_queues; i++)
    {
        pq_free(&queues[i]);
    }
    free(queues);
    free(admitted_processes);

    return 0;
}