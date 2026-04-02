/*
 * file: stcf.c
 * Shortest time-to-completion first scheduling
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/scheduler.h"
#include "../include/gantt.h"
#include "../include/metrics.h"
#include "../include/sched_utils.h"

int schedule_stcf(SchedulerState *state)
{
    // Implementation for STCF scheduling
    SchedulerContext ctx;
    ctx_init(&ctx, state);

    for (int i = 0; i < ctx.num_processes; i++)
    {
       int arrival_time = state->processes[i].arrival_time;
       if (ctx.time == 0 || arrival_time < ctx.time){
        ctx.time = arrival_time;
       }
    }

    while (ctx.completed < ctx.num_processes)
    {
        int shortest_remaining_time = -1;

        for (int i = 0; i < ctx.num_processes; i++)
        {
            Process *proc = &state->processes[i];
            if (proc->arrival_time <= ctx.time && proc->remaining_time > 0)
            {
                if (shortest_remaining_time == -1 || proc->remaining_time < state->processes[shortest_remaining_time].remaining_time)
                {
                    shortest_remaining_time = i;
                }
            }
        }

        if (shortest_remaining_time == -1)
        {
            int next_process = -1;
            for (int i = 0; i < ctx.num_processes; i++)
            {
                Process *proc = &state->processes[i];
                if (proc->remaining_time > 0 && proc->arrival_time > ctx.time)
                {
                    if (next_process == -1 || proc->arrival_time < next_process)
                    {
                        next_process = proc->arrival_time;
                    }
                }
            }

                if (next_process != -1)
                {
                    gantt_extend(state, "IDLE", ctx.time, next_process);
                    ctx.time = next_process; // Move time forward to the next process arrival
                }
                else
                {
                    break; // No more processes to run
                }
                continue; // No process ready, move time forward to the next arrival time
            }

        Process *proc = &state->processes[shortest_remaining_time];
        if (proc->start_time == -1)
        {
            log_process_start(ctx.time, proc->pid);
            proc->start_time = ctx.time; // First time the process is executed
        }

        ctx_track_switch(state, &ctx, proc->pid);

        proc->remaining_time--;
        gantt_extend(state, proc->pid, ctx.time, ctx.time + 1);
        ctx.time++;

        if (proc->remaining_time == 0)
        {
            proc->finish_time = ctx.time;
            log_process_finish(ctx.time, proc->pid);
            ctx.completed++;
        }
    }
    finish_scheduler(state);
    return 0;
}