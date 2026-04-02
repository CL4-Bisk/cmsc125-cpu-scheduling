/*
 * file: rr.c
 * Round-robin scheduling
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/scheduler.h"
#include "../include/gantt.h"
#include "../include/metrics.h"
#include "../include/sched_utils.h"

int schedule_rr(SchedulerState *state, int quantum)
{
    // Implementation for Round-robin scheduling
    SchedulerContext ctx;
    ctx_init(&ctx, state);

    // 1. Initialize the ready queue
    IntQueue iq;
    iq_init(&iq, ctx.num_processes + 1);
    int *admitted_processes = calloc(ctx.num_processes, sizeof(int));

    while (ctx.completed < ctx.num_processes)
    {
        // 1. Admit processes that arrived at or before the current 'time'
        for (int i = 0; i < ctx.num_processes; i++)
        {
            if (!admitted_processes[i] && state->processes[i].arrival_time <= ctx.time)
            {
                iq_push(&iq, i);
                admitted_processes[i] = 1;
            }
        }

        if (iq.size == 0)
        {
            int next_arrival = ctx_next_arrival(state, &ctx, admitted_processes);

            if (next_arrival == -1)
            {
                break;
            }
            ctx_handle_idle(state, &ctx, next_arrival);
            continue; // No processes ready, move time forward to next arrival
        }

        // 2. Pop the next process
        int index = iq_pop(&iq);
        Process *proc = &state->processes[index];

        // 3. Record Start Time / Context Switch
        if (proc->start_time == -1)
        {
            proc->start_time = ctx.time;
        }
        ctx_track_switch(state, &ctx, proc->pid);

        // 4. Run for 1 quantum (or remaining time)
        int run_time = (proc->remaining_time < quantum) ? proc->remaining_time : quantum;
        gantt_extend(state, proc->pid, ctx.time, ctx.time + run_time);

        proc->remaining_time -= run_time;
        ctx.time += run_time;

        // 5. Check if finished or needs to go back to queue
        if (proc->remaining_time > 0)
        {
            iq_push(&iq, index);
        }
        else
        {
            proc->finish_time = ctx.time;
            log_process_finish(ctx.time, proc->pid);
            ctx.completed++;
        }
    }

    // Cleanup
    iq_free(&iq);
    free(admitted_processes);
    finish_scheduler(state);
    return 0;
}
