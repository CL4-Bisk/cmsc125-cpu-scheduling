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
    int *admitted_processes = calloc(num_processes, sizeof(int)); // Track admitted processes

    // 2. Start the simulation
    int first_arrival = state->processes[0].arrival_time;
    for (int i = 0; i < num_processes; i++)
    {
        if (state->processes[i].arrival_time < first_arrival)
        {
            first_arrival = state->processes[i].arrival_time;
        }
    }

    if (first_arrival > time)
    {
        time = first_arrival; // Start at the arrival time of the first process
    }

    while (completed < state->num_processes)
    {
        // 3. Admit new processes to the ready queue
        for (int i = 0; i < num_processes; i++)
        {
            if (!admitted_processes[i] && state->processes[i].arrival_time <= time)
            {
                iq_push(&iq, i);
                admitted_processes[i] = 1;
            }
        }

        // 4. Execute processes in the ready queue
        if (iq.size == 0)
        {
            int next_arrival = -1;
            for (int i = 0; i < num_processes; i++)
            {
                if (!admitted_processes[i] && (next_arrival == -1 || state->processes[i].arrival_time < next_arrival))
                {
                    next_arrival = state->processes[i].arrival_time;
                }
            }
            if (next_arrival != -1)
            {
                time = next_arrival; // Move time forward to next arrival
            }
            continue; // No processes ready, move time forward to next arrival
        }

        // 5. Get the next process from the ready queue
        int index = iq_pop(&iq);
        Process *proc = &state->processes[index];
        if (proc->start_time == -1)
        {
            log_process_start(time, proc->pid);
            proc->start_time = time; // First time the process is executed
        }

        if (last_pid[0] != '\0' && strcmp(last_pid, proc->pid) != 0)
        {
            state->context_switches++; // Increment context switch count if switching to a different process
            log_context_switch(time, last_pid, proc->pid);
        }
        strncpy(last_pid, proc->pid, 15);
        last_pid[15] = '\0';

        // 6. Run the process for the time quantum or until completion
        int run_time = (proc->remaining_time < quantum) ? proc->remaining_time : quantum;
        int run_start = time;
        proc->remaining_time -= run_time;
        time += run_time;
        gantt_add_or_extend(state, proc->pid, run_start, time);

        // 7. Move time forward by the run time
        for (int i = 0; i < num_processes; i++)
        {
            if (!admitted_processes[i] && state->processes[i].arrival_time <= time)
            {
                iq_push(&iq, i);
                admitted_processes[i] = 1;
            }
        }

        if (proc->remaining_time == 0)
        {
            proc->finish_time = time;
            log_process_finish(time, proc->pid);
            completed++;
        }
        else
        {
            iq_push(&iq, index); // Re-add to the end of the ready queue if not finished
        }
    }

    // Cleanup
    iq_free(&iq);
    free(admitted_processes);
    calculate_metrics(state);
    gantt_print(state);
    return 0;
}