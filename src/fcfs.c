/*
 * file: fcfs.c
 * First come, first serve scheduling
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/scheduler.h"
#include "../include/gantt.h"
#include "../include/metrics.h"
#include "../include/sched_utils.h"

int schedule_fcfs(SchedulerState *state)
{
    // Implementation for FCFS scheduling
    sort_processes(state->processes, state->num_processes);

    SchedulerContext ctx;
    ctx_init(&ctx, state);

    for (int i = 0; i < ctx.num_processes; i++)
    {
        Process *proc = &state->processes[i];
        if (proc->arrival_time > ctx.time)
        {
            ctx_handle_idle(state, &ctx, proc->arrival_time);
        }

        ctx_track_switch(state, &ctx, proc->pid);

        log_process_start(ctx.time, proc->pid);
        proc->start_time = ctx.time;
        proc->finish_time = ctx.time + proc->burst_time; // Run the process to completion
        gantt_add_entry(state, proc->pid, ctx.time, proc->finish_time);
        log_process_finish(proc->finish_time, proc->pid);
        ctx.time += proc->burst_time; // Move time forward by the burst time
    }
    finish_scheduler(state);
    print_logs();
    // Convoy effect
    for (int i = 0; i < ctx.num_processes; i++)
    {
        Process *proc = &state->processes[i];
        if (proc->waiting_time > 2 * proc->burst_time)
        {
            fprintf(stderr, "Warning: Process %s is experiencing the convoy effect (waiting time: %d)\n",
                    proc->pid, proc->waiting_time);
        }
    }

    return 0;
}