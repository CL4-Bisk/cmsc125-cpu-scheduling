/*
 * file: main.c
 * CLI and main loop
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../include/process.h"
#include "../include/scheduler.h"

int main(int argc, char *argv[])
{
    char algorithm[16] = "";
    char input_file[256]  = "";
    char processes_str[1024] = "";
    int  quantum = DEFAULT_QUANTUM;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--algorithm=", 12) == 0) {
            strncpy(algorithm, argv[i] + 12, sizeof(algorithm) - 1);
        } else if (strncmp(argv[i], "--input=", 8) == 0) {
            strncpy(input_file, argv[i] + 8, sizeof(input_file) - 1);
        } else if (strncmp(argv[i], "--processes=", 12) == 0) {
            strncpy(processes_str, argv[i] + 12, sizeof(processes_str) - 1);
        } else if (strncmp(argv[i], "--quantum=", 10) == 0) {
            quantum = atoi(argv[i] + 10);
            if (quantum <= 0) {
                fprintf(stderr, "Error: Quantum must be a positive integer.\n");
                return 1;
            }
        } else if (strncmp(argv[i], "--comparison=", 14) == 0) {
            strncpy(processes_str, argv[i] + 12, sizeof(processes_str) - 1);
        } else {
            fprintf(stderr, "Unknown/Missing argument(s): %s\n", argv[i]);
            return 1;
        }
    }
        
    if (strlen(input_file) == 0 && strlen(processes_str) == 0) {
        fprintf(stderr, "Error: No workload provided. Use --input or --processes.\n");
        return 1;
    }

    int num_processes = 0;
    Process *processes = load_processes(input_file, processes_str, &num_processes);
    if (processes == NULL) {
        return 1;
    }

    SchedulerState state = {
        .processes = processes,
        .num_processes = num_processes,
        .current_time  = 0,
        .context_switches = 0,
        .gantt_head      = NULL,
        .gantt_tail      = NULL
    };

    if (strcmp(algorithm, "FCFS") == 0) {
        printf("Running FCFS Scheduler...\n");
        schedule_fcfs(&state);

    } else if (strcmp(algorithm, "SJF") == 0) {
        printf("Running SJF Scheduler...\n");
        schedule_sjf(&state);

    } else if (strcmp(algorithm, "STCF") == 0) {
        printf("Running STCF Scheduler...\n");
        schedule_stcf(&state);

    } else if (strcmp(algorithm, "RR") == 0) {
        printf("Running Round Robin Scheduler (quantum=%d)...\n", quantum);
        schedule_rr(&state, quantum);

    } else if (strcmp(algorithm, "MLFQ") == 0) {
        MLFQQueue mlfq_queues[3] = {
            { .level = 0, .time_quantum = 10, .allotment = 50,  .queue = NULL, .size = 0 },
            { .level = 1, .time_quantum = 30, .allotment = 150, .queue = NULL, .size = 0 },
            { .level = 2, .time_quantum = -1, .allotment = -1,  .queue = NULL, .size = 0 }
        };

        MLFQScheduler mlfq = {
            .queues       = mlfq_queues,
            .num_queues   = 3,
            .boost_period = 200,
            .last_boost   = 0
        };
        printf("Running MLFQ Scheduler...\n");
        schedule_mlfq(&state, &mlfq);

    } else {
        fprintf(stderr, "Error: Unknown algorithm '%s'\n", algorithm);
        fprintf(stderr, "Valid options: FCFS, SJF, STCF, RR, MLFQ\n");
        return 1;
    }
    gantt_free(&state);
    reset_processes(processes, num_processes);
    free(processes);
    return 0;
}

