/*
 * file: scheduler_utils.c
 * Shared helpers used by all schedulers
 */
#include <stdlib.h>
#include <string.h>
#include "../include/sched_utils.h"
#include "../include/gantt.h"
#include "../include/metrics.h"

/* ── SchedulerContext ───────────────────────────────────────────────────── */

void ctx_init(SchedulerContext *ctx, SchedulerState *state)
{
    ctx->time = 0;
    ctx->completed = 0;
    ctx->num_processes = state->num_processes;
    ctx->last_pid[0] = '\0';
}

void ctx_track_switch(SchedulerState *state, SchedulerContext *ctx, const char *new_pid)
{
    if (ctx->last_pid[0] != '\0' && strcmp(ctx->last_pid, new_pid) != 0)
    {
        state->context_switches++;
        log_context_switch(ctx->time, ctx->last_pid, new_pid);
    }
    strncpy(ctx->last_pid, new_pid, 15);
    ctx->last_pid[15] = '\0';
}

int ctx_next_arrival(SchedulerState *state, SchedulerContext *ctx, const int *admitted)
{
    int next = -1;
    for (int i = 0; i < ctx->num_processes; i++)
    {
        if (!admitted[i] && state->processes[i].arrival_time > ctx->time)
        {
            if (next == -1 || state->processes[i].arrival_time < next)
                next = state->processes[i].arrival_time;
        }
    }
    return next;
}

void ctx_handle_idle(SchedulerState *state, SchedulerContext *ctx, int next_arrival)
{
    gantt_add_entry(state, "IDLE", ctx->time, next_arrival);
    log_idle_interval(ctx->time, next_arrival);
    ctx->time = next_arrival;
}

void finish_scheduler(SchedulerState *state)
{
    calculate_metrics(state);
    gantt_print(state);
}

/* ── IntQueue ───────────────────────────────────────────────────────────── */

void iq_init(IntQueue *iq, int capacity)
{
    iq->data = malloc(capacity * sizeof(int));
    iq->head = 0;
    iq->tail = 0;
    iq->size = 0;
    iq->capacity = capacity;
}

void iq_push(IntQueue *iq, int value)
{
    if (iq->size < iq->capacity)
    {
        iq->data[iq->tail] = value;
        iq->tail = (iq->tail + 1) % iq->capacity;
        iq->size++;
    }
}

int iq_pop(IntQueue *iq)
{
    if (iq->size == 0)
        return -1;
    int value = iq->data[iq->head];
    iq->head = (iq->head + 1) % iq->capacity;
    iq->size--;
    return value;
}

void iq_free(IntQueue *iq)
{
    free(iq->data);
    iq->data = NULL;
    iq->head = 0;
    iq->tail = 0;
    iq->size = 0;
    iq->capacity = 0;
}

/* ── ProcessQueue ───────────────────────────────────────────────────────── */

void pq_init(ProcessQueue *pq, int capacity)
{
    pq->processes = malloc(capacity * sizeof(Process *));
    pq->size = 0;
    pq->capacity = capacity;
}

void pq_push(ProcessQueue *pq, Process *process)
{
    if (pq->size == pq->capacity)
    {
        pq->capacity *= 2;
        pq->processes = realloc(pq->processes, pq->capacity * sizeof(Process *));
    }
    pq->processes[pq->size++] = process;
}

Process *pq_pop(ProcessQueue *pq)
{
    if (pq->size == 0)
        return NULL;
    Process *proc = pq->processes[0];
    memmove(pq->processes, pq->processes + 1, (pq->size - 1) * sizeof(Process *));
    pq->size--;
    return proc;
}

void pq_free(ProcessQueue *pq)
{
    free(pq->processes);
    pq->processes = NULL;
    pq->size = 0;
    pq->capacity = 0;
}