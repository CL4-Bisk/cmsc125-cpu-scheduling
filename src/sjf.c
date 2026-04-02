/*
 * file: sjf.c
 * Shortest job first scheduling
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/gantt.h"
#include "../include/scheduler.h"
#include "../include/metrics.h"
#include "../include/sched_utils.h"

int schedule_sjf(SchedulerState *state)
{
    // Implementation for Shortest job first scheduling
    SchedulerContext ctx;
    ctx_init(&ctx, state);

    while (ctx.completed < ctx.num_processes)
    {
        int shortest_burst_time = -1;

        for (int i = 0; i < ctx.num_processes; i++)
        {
            Process *proc = &state->processes[i];
            if (proc->arrival_time <= ctx.time && proc->remaining_time > 0)
            {
                if (shortest_burst_time == -1 || proc->remaining_time < state->processes[shortest_burst_time].remaining_time)
                {
                    shortest_burst_time = i;
                }
            }
        }

        if (shortest_burst_time == -1)
        {
            int next_arrival = -1;
            for (int i = 0; i < ctx.num_processes; i++)
            {
                Process *proc = &state->processes[i];
                if (proc->remaining_time > 0 && proc->arrival_time > ctx.time)
                {
                    if (next_arrival == -1 || proc->arrival_time < next_arrival)
                    {
                        next_arrival = proc->arrival_time;
                    }
                }
            }

            if (next_arrival != -1)
            {
                ctx_handle_idle(state, &ctx, next_arrival);
            }
            else
            {
                break; // no more processes
            }
            continue;
        }
        
        Process *proc = &state->processes[shortest_burst_time];

        ctx_track_switch(state, &ctx, proc->pid);

        log_process_start(ctx.time, proc->pid);
        proc->start_time = ctx.time;
        ctx.time += proc->remaining_time;
        proc->finish_time = ctx.time;
        gantt_add_entry(state, proc->pid, proc->start_time, proc->finish_time);
        log_process_finish(proc->finish_time, proc->pid);
        proc->remaining_time = 0;
        ctx.completed++;
        
    }
    finish_scheduler(state);
    return 0;
}