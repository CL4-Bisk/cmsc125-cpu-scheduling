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

int schedule_rr(SchedulerState *state, int quantum)
{
    // Implementation for Round-robin scheduling
    int time = 0;
    int completed = 0;
    int num_processes = state->num_processes;

    for (int i = 0; i < num_processes; i++)
    {
        state->processes[i].remaining_time = state->processes[i].burst_time;
    }

    while (completed < state->num_processes)
    {
        for (int i = 0; i < state->num_processes; i++)
        {
            Process *proc = &state->processes[i];
            if (proc->arrival_time <= time && proc->remaining_time > 0)
            {

                if (proc->remaining_time == proc->burst_time)
                {
                    proc->start_time = time; // First time the process is executed
                }

                if (proc->remaining_time > quantum)
                {
                    time += quantum;
                    proc->remaining_time -= quantum;
                }
                else
                {
                    time += proc->remaining_time;
                    proc->finish_time = time;
                    proc->remaining_time = 0;
                    completed++;
                }
            }
        }
    }
    return 0;
}