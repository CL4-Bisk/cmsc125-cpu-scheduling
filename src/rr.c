/*
 * file: rr.c
 * Round-robin scheduling
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/scheduler.h"
#include "../include/gantt.h"
#include "../include/metrics.h"

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
    {
        return -1;
    }
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

int schedule_rr(SchedulerState *state, int quantum)
{
    // Implementation for Round-robin scheduling
    int time = 0;
    int completed = 0;
    int num_processes = state->num_processes;
    char last_pid[16] = "";

    // 1. Initialize the ready queue
    IntQueue iq;
    iq_init(&iq, num_processes + 1);
    int *admitted_processes = calloc(num_processes, sizeof(int));

    while (completed < num_processes)
    {
        // 1. Admit processes that arrived at or before the current 'time'
        for (int i = 0; i < num_processes; i++)
        {
            if (!admitted_processes[i] && state->processes[i].arrival_time <= time)
            {
                iq_push(&iq, i);
                admitted_processes[i] = 1;
            }
        }

        if (iq.size == 0)
        {
            int next_arrival = -1;
            for (int i = 0; i < num_processes; i++)
            {
                if (!admitted_processes[i] && state->processes[i].arrival_time > time)
                {
                    if (next_arrival == -1 || state->processes[i].arrival_time < next_arrival)
                    {
                        next_arrival = state->processes[i].arrival_time;
                    }
                }
            }
            if (next_arrival != -1)
            {
                gantt_add_entry(state, "IDLE", time, next_arrival);
                log_idle_interval(time, next_arrival);
                time = next_arrival; // Move time forward to next arrival
            }
            continue; // No processes ready, move time forward to next arrival
        }

        // 2. Pop the next process
        int index = iq_pop(&iq);
        Process *proc = &state->processes[index];

        // 3. Record Start Time / Context Switch
        if (proc->start_time == -1)
        {
            proc->start_time = time;
        }
        if (last_pid[0] != '\0' && strcmp(last_pid, proc->pid) != 0)
        {
            state->context_switches++;
        }
        strncpy(last_pid, proc->pid, 15);

        // 4. Run for 1 quantum (or remaining time)
        int run_time = (proc->remaining_time < quantum) ? proc->remaining_time : quantum;
        gantt_extend(state, proc->pid, time, time + run_time);

        proc->remaining_time -= run_time;
        int end_time = time + run_time;

        // 5. Check if finished or needs to go back to queue
        if (proc->remaining_time > 0)
        {
            iq_push(&iq, index);
        }
        else
        {
            proc->finish_time = end_time;
            log_process_finish(end_time, proc->pid);
            completed++;
        }

        time = end_time;
    }

    // Cleanup
    iq_free(&iq);
    free(admitted_processes);
    calculate_metrics(state);
    gantt_print(state);
    return 0;
}
