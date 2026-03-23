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

int schedule_stcf(SchedulerState *state)
{
    // Implementation for STCF scheduling

    int time = 0;
    int completed = 0;
    int num_processes = state->num_processes;

    for (int i = 0; i < num_processes; i++)
    {
        state->processes[i].remaining_time = state->processes[i].burst_time;
    }

    while (completed < num_processes)
    {
        int shortest_remaining_time = -1;
        int shortest_remaining_time_index = -1;

        for (int i = 0; i < num_processes; i++)
        {
            if (state->processes[i].arrival_time <= time && state->processes[i].remaining_time > 0)
            {
                if (shortest_remaining_time == -1 || state->processes[i].remaining_time < shortest_remaining_time)
                {
                    shortest_remaining_time = state->processes[i].remaining_time;
                    shortest_remaining_time_index = i;
                }
            }
        }

        if (shortest_remaining_time_index == -1)
        {
            time++;
        }
        else
        {
            Process *proc = &state->processes[shortest_remaining_time_index];
            if (proc->remaining_time == proc->burst_time)
            {
                proc->start_time = time; // First time the process is executed
            }
            proc->remaining_time--;
            time++;

            if (proc->remaining_time == 0)
            {
                proc->finish_time = time;
                completed++;
            }
        }
    }
    calculate_metrics(state);
    return 0;
}