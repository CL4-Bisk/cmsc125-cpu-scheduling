#ifndef METRICS_H
#define METRICS_H

#include "process.h"
#include "scheduler.h"

void calculate_metrics(SchedulerState *state);
void print_metrics(SchedulerState *state);

#endif