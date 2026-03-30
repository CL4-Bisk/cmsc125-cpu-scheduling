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
    sort_processes(state->processes, state->num_processes);

    int time = 0;
    char last_pid[16] = "";

    for (int i = 0; i < state->num_processes; i++)
    {
        Process *proc = &state->processes[i];
        if (proc->arrival_time > time)
        {
            gantt_add_entry(state, "IDLE", time, proc->arrival_time);
            log_idle_interval(time, proc->arrival_time);
            time = proc->arrival_time; // Wait for the process to arrive
        }

        if (last_pid[0] != '\0' && strcmp(last_pid, proc->pid) != 0)
        {
            state->context_switches++; // Increment context switch count if switching to a different process
            log_context_switch(time, last_pid, proc->pid);
        }
        strncpy(last_pid, proc->pid, 15);
        last_pid[15] = '\0';

        log_process_start(time, proc->pid);
        proc->start_time = time;
        proc->finish_time = time + proc->burst_time; // Run the process to completion
        gantt_add_entry(state, proc->pid, time, proc->finish_time);
        log_process_finish(proc->finish_time, proc->pid);
        time += proc->burst_time; // Move time forward by the burst time
    }
    calculate_metrics(state);
    gantt_print(state);

    // Convoy effect
    for (int i = 0; i < state->num_processes; i++)
    {
        Process *proc = &state->processes[i];
        if (proc->waiting_time > 2 * proc->burst_time)
        {
            fprintf(stderr, "Warning: Process %s is experiencing the convoy effect (waiting time: %d)\n",
                    proc->pid, proc->waiting_time);
        }
    }

    return 0;
}