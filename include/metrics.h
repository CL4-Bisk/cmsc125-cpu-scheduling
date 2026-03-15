#ifndef METRICS_H
#define METRICS_H

#include "process.h"
#include "scheduler.h"
/* prevents the header file to be compiled multiple times */

typedef struct {
    double turnaround_time; // Finish Time - Arrival Time
    double waiting_time;    // Turnaround Time - Burst Time
    double response_time;   // Start Time - Arrival Time
} ProcessMetrics;

//// add struct for overall metrics if needed
// typedef struct {
//     double average_turnaround_time;
//     double average_waiting_time;
//     double average_response_time;
// } OverallMetrics;

void calculate_metrics(SchedulerState *state, ProcessMetrics *metrics);

#endif