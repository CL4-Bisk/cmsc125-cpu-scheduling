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

int schedule_sjf(SchedulerState *state)
{
    // Implementation for Shortest job first scheduling

    int time = 0;
    int completed = 0;
    int num_processes = state->num_processes;

    while (completed < num_processes)
    {
        int shortest_burst_time = -1;
        int shortest_burst_time_index = -1;

        for (int i = 0; i < num_processes; i++)
        {
            if (state->processes[i].arrival_time <= time && state->processes[i].remaining_time > 0)
            {
                if (shortest_burst_time == -1 || state->processes[i].remaining_time < shortest_burst_time)
                {
                    shortest_burst_time = state->processes[i].remaining_time;
                    shortest_burst_time_index = i;
                }
            }
        }

        if (shortest_burst_time_index == -1)
        {
            time++;
        }
        else
        {
            Process *proc = &state->processes[shortest_burst_time_index];
            proc->start_time = time;
            time += proc->remaining_time;
            proc->finish_time = time;
            proc->remaining_time = 0;
            completed++;
        }
    }
    calculate_metrics(state);
    return 0;
}