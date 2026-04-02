#ifndef GANTT_H
#define GANTT_H

#define MAX_CHART_WIDTH 60

typedef struct GanttEntry
{
    char pid[16];
    int start_time;
    int end_time;
    struct GanttEntry *next;
} GanttEntry;

typedef struct SchedulerState SchedulerState;

void gantt_add_entry(SchedulerState *state, const char *pid, int start, int end);
void gantt_extend(SchedulerState *state, const char *pid, int start, int end);
void gantt_print(SchedulerState *state);
void gantt_free(SchedulerState *state);

#endif