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

int schedule_fcfs(SchedulerState *state)
{
    // Implementation for FCFS scheduling
    
    for (int i = 0; i < state->num_processes - 1; i++) {
        for (int j = 0; j < state->num_processes - i - 1; j++) {
            if (state->processes[j].arrival_time > state->processes[j+1].arrival_time) {
                Process temp = state->processes[j];
                state->processes[j] = state->processes[j+1];
                state->processes[j+1] = temp;
            }
        }
    }

    int time = 0;
    for (int i = 0; i < state->num_processes; i++)
    {
        Process *proc = &state->processes[i];
        if (proc->arrival_time > time)
        {
            time = proc->arrival_time; // Wait for the process to arrive
        }
        proc->start_time = time;
        proc->finish_time = time + proc->burst_time;                // Run the process to completion
        time += proc->burst_time;                                   // Move time forward by the burst time
    }
    calculate_metrics(state);
    return 0;
}