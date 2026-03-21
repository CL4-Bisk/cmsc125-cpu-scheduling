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
    int remaining_times[state->num_processes];

    for (int i = 0; i < state->num_processes; i++)
    {
        remaining_times[i] = state->processes[i].burst_time;
    }

    while (completed < state->num_processes)
    {
        for (int i = 0; i < state->num_processes; i++)
        {
            Process *proc = &state->processes[i];
            if (proc->arrival_time <= time && remaining_times[i] > 0)
            {
                if (remaining_times[i] > quantum)
                {
                    time += quantum;
                    remaining_times[i] -= quantum;
                }
                else
                {
                    time += remaining_times[i];
                    proc->finish_time = time;
                    remaining_times[i] = 0;
                    completed++;
                }
            }
        }
    }
    return 0;
}