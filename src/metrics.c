/*
 * file: metrics.c
 * Functions to calculate and display process metrics (turnaround time, waiting time, response time)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/metrics.h"

void calculate_metrics(SchedulerState* state)
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

        p->turnaround_time = p->finish_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        p->response_time = p->start_time - p->arrival_time;
    }
    print_metrics(state);
}

void print_metrics(SchedulerState *state)
{
    // Implementation to print metrics for each process and overall averages
    // This function iterates through the processes in the SchedulerState, prints the turnaround time, waiting time, and response time for each process, and the overall average metrics if needed.
    double total_tt = 0, total_wt = 0, total_rt = 0;
    int n = state->num_processes;

    printf("\n=== Metrics ===\n\n");
    printf("%-10s | %-5s | %-5s | %-5s | %-5s | %-5s | %-5s\n",
           "Process", "AT", "BT", "FT", "TT", "WT", "RT");
    printf("-----------|-------|-------|-------|-------|-------|------\n");

    for (int i = 0; i < n; i++) {
        Process *p = &state->processes[i];

        printf("%-10s | %-5d | %-5d | %-5d | %-5d | %-5d | %-5d\n",
               p->pid,
               p->arrival_time,
               p->burst_time,
               p->finish_time,
               p->turnaround_time,
               p->waiting_time,
               p->response_time);

        total_tt += p->turnaround_time;
        total_wt += p->waiting_time;
        total_rt += p->response_time;
    }

    printf("-----------|-------|-------|-------|-------|-------|------\n");
    printf("%-10s | %-5s | %-5s | %-5s | %-5.1f | %-5.1f | %-5.1f\n",
           "Average", "", "", "",
           total_tt / n,
           total_wt / n,
           total_rt / n);
    printf("\n");
}
