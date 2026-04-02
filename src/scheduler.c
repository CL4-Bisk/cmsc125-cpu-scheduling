#include <stdio.h>
#include <string.h>
#include "../include/scheduler.h"

static int run_fcfs(SchedulerState *s, int q) { return schedule_fcfs(s); }
static int run_sjf(SchedulerState *s, int q) { return schedule_sjf(s); }
static int run_stcf(SchedulerState *s, int q) { return schedule_stcf(s); }
static int run_rr(SchedulerState *s, int q) { return schedule_rr(s, q); }

static int run_mlfq(SchedulerState *s, int q)
{
    MLFQQueue mlfq_queues[3] = {
        {.level = 0, .time_quantum = 10, .allotment = 50, .queue = NULL, .size = 0},
        {.level = 1, .time_quantum = 30, .allotment = 150, .queue = NULL, .size = 0},
        {.level = 2, .time_quantum = -1, .allotment = -1, .queue = NULL, .size = 0},
    };
    MLFQScheduler mlfq = {
        .queues = mlfq_queues,
        .num_queues = 3,
        .boost_period = 200,
        .last_boost = 0};
    return schedule_mlfq(s, &mlfq);
}

static const SchedulerEntry schedulers[] = {
    {"FCFS", run_fcfs},
    {"SJF", run_sjf},
    {"STCF", run_stcf},
    {"RR", run_rr},
    {"MLFQ", run_mlfq},
};
static const int NUM_SCHEDULERS = sizeof(schedulers) / sizeof(schedulers[0]);

int run_scheduler(SchedulerState *state, const char *algorithm, int quantum)
{
    for (int i = 0; i < NUM_SCHEDULERS; i++)
    {
        if (strcmp(algorithm, schedulers[i].name) == 0)
        {
            printf("Running %s Scheduler...\n", algorithm);
            return schedulers[i].run(state, quantum);
        }
    }

    fprintf(stderr, "Error: Unknown algorithm '%s'\n", algorithm);
    fprintf(stderr, "Valid options:");
    for (int i = 0; i < NUM_SCHEDULERS; i++)
        fprintf(stderr, " %s", schedulers[i].name);
    fprintf(stderr, "\n");
    return -1;
}