#ifndef GANTT_H
#define GANTT_H
/* prevents the header file to be compiled multiple times */

#include "process.h"
#include "scheduler.h"

void print_gantt_chart(SchedulerState *state);

#endif