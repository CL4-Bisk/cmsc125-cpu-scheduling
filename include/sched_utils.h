#ifndef SCHEDULER_UTILS_H
#define SCHEDULER_UTILS_H

#include "scheduler.h"

// Runtime state shared by every scheduler — replaces the 4 repeated locals
typedef struct
{
    int time;
    int completed;
    int num_processes;
    char last_pid[16];
} SchedulerContext;

// Initialise context from state
void ctx_init(SchedulerContext *ctx, SchedulerState *state);

// Track a context switch and update last_pid.
// Reads ctx->time for the log timestamp.
void ctx_track_switch(SchedulerState *state, SchedulerContext *ctx, const char *new_pid);

// Scan admitted[] for the earliest unadmitted arrival after ctx->time.
// Returns -1 if none remain.
int ctx_next_arrival(SchedulerState *state, SchedulerContext *ctx, const int *admitted);

// Add an IDLE gantt entry and advance ctx->time to next_arrival.
void ctx_handle_idle(SchedulerState *state, SchedulerContext *ctx, int next_arrival);

// Call calculate_metrics + gantt_print. Every scheduler ends with this.
void finish_scheduler(SchedulerState *state);

// IntQueue helpers (used by rr.c)
void iq_init(IntQueue *iq, int capacity);
void iq_push(IntQueue *iq, int value);
int iq_pop(IntQueue *iq);
void iq_free(IntQueue *iq);

// ProcessQueue helpers (used by mlfq.c)
void pq_init(ProcessQueue *pq, int capacity);
void pq_push(ProcessQueue *pq, Process *process);
Process *pq_pop(ProcessQueue *pq);
void pq_free(ProcessQueue *pq);

#endif