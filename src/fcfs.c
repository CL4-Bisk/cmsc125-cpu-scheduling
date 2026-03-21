/*
 * file: fcfs.c
 * First come, first serve scheduling
 */
#include <stdio.h>
#include <stdlib.h>
#include "../include/gantt.h"
#include "../include/scheduler.h"

int schedule_fcfs(SchedulerState *state)
{
    // Implementation for FCFS scheduling
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
        proc->waiting_time = proc->start_time - proc->arrival_time; // Calculate waiting time
        time += proc->burst_time;                                   // Move time forward by the burst time
    }
    return 0;
}