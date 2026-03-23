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
    char last_pid[16] = "";

    int first_arrival = state->processes[0].arrival_time;
    for (int i = 0; i < num_processes; i++)
    {
        if (state->processes[i].arrival_time < first_arrival)
        {
            first_arrival = state->processes[i].arrival_time;
        }
    }

    if (first_arrival > 0)
    {
        time = first_arrival; // Start at the arrival time of the first process
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
                if (shortest_remaining_time == -1 || proc->remaining_time < shortest_remaining_time)
                {
                    shortest_remaining_time = proc->remaining_time;
                    shortest_remaining_time_index = i;
                }
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

        Process *proc = &state->processes[shortest_remaining_time_index];
        if (proc->start_time == -1)
        {
            proc->start_time = time; // First time the process is executed
        }

        if (last_pid[0] != '\0' && strcmp(last_pid, proc->pid) != 0)
        {
            state->context_switches++; // Increment context switch count if switching to a different process
        }
        strncpy(last_pid, proc->pid, 15);
        last_pid[15] = '\0';

        proc->remaining_time--;
        time++;

        if (proc->remaining_time == 0)
        {
            proc->finish_time = time;
            completed++;
        }
    }
    calculate_metrics(state);
    return 0;
}