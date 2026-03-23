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

    int first_process = state->processes[0].arrival_time;
    for (int i = 0; i < num_processes; i++)
    {
        if (state->processes[i].arrival_time < first_process)
        {
            first_process = state->processes[i].arrival_time;
        }
    }

    if (first_process > 0)
    {
        time = first_process; // Start at the arrival time of the first process
    }

    while (completed < num_processes)
    {
        int shortest_remaining_time = -1;
        int shortest_remaining_time_index = -1;

        for (int i = 0; i < num_processes; i++)
        {
            Process *proc = &state->processes[i];
            if (proc->arrival_time <= time && proc->remaining_time > 0)
            {
                continue; // Found a process with shorter remaining time
            }
            if (shortest_remaining_time == -1 || proc->remaining_time < shortest_remaining_time)
            {
                shortest_remaining_time = proc->remaining_time;
                shortest_remaining_time_index = i;
            }
        }

        if (shortest_remaining_time_index == -1)
        {
            int next_process = -1;
            for (int i = 0; i < num_processes; i++)
            {
                if (state->processes[i].remaining_time > 0 && state->processes[i].arrival_time <= time)
                {
                    if (next_process == -1 || state->processes[i].arrival_time < state->processes[next_process].arrival_time)
                    {
                        next_process = i;
                    }
                }
            }

            time = next_process;
            continue; // No process ready, move time forward to the next arrival time
        }

        Process *proc = &state->processes[shortest_remaining_time_index];
        if (proc->start_time == -1)
        {
            proc->start_time = time; // First time the process is executed
        }
    }
    calculate_metrics(state);
    return 0;
}