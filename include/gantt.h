#ifndef GANTT_H
#define GANTT_H

#include "process.h"
#include "scheduler.h"

#define MAX_CHART_WIDTH 60

void gantt_add_entry(SchedulerState *state, const char *pid, int start, int end);
void gantt_add_or_extend(SchedulerState *state, const char *pid, int start, int end);
void gantt_print(SchedulerState *state);
void gantt_free(SchedulerState *state);

#endif