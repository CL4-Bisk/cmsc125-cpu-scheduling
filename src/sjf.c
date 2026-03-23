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
    char last_pid[16] = "";

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

            if (next_process != -1)
            {
                time = next_process; // Move time forward to the next process arrival
            }
            else
            {
                break; // No more processes to run
            }
            continue; // No process ready, move time forward to the next arrival time
        }
        else
        {
            Process *proc = &state->processes[shortest_burst_time_index];

            if (last_pid[0] != '\0' && strcmp(last_pid, proc->pid) != 0)
            {
                state->context_switches++; // Increment context switch count if switching to a different process
            }
            strncpy(last_pid, proc->pid, 15);
            last_pid[15] = '\0';

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