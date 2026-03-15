/*
 * file: metrics.c
 * Functions to calculate and display process metrics (turnaround time, waiting time, response time)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/metrics.h"

void calculate_metrics(SchedulerState *state)
{
    /*
     * Implementation to calculate metrics for each process and overall averages.
     *
     * This function iterates through the processes in the SchedulerState,
     * calculates the turnaround time, waiting time, and response time for each process,
     * and stores these values in the provided ProcessMetrics array. It can also calculate
     * the overall average metrics if needed.
     */

    for (int i = 0; i < state->num_processes; i++)
    {
        Process *p = &state->processes[i];
        int tt = p->finish_time - p->arrival_time;
        p->waiting_time = tt - p->burst_time;
    }
}

void print_metrics(SchedulerState *state)
{
    // Implementation to print metrics for each process and overall averages
    // This function iterates through the processes in the SchedulerState, prints the turnaround time, waiting time, and response time for each process, and the overall average metrics if needed.
}