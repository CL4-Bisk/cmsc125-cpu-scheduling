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
    char input_file[256] = "";
    char processes_str[1024] = "";
    int quantum = DEFAULT_QUANTUM;

    for (int i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], "--algorithm=", 12) == 0)
        {
            strncpy(algorithm, argv[i] + 12, sizeof(algorithm) - 1);
        }
        else if (strncmp(argv[i], "--input=", 8) == 0)
        {
            strncpy(input_file, argv[i] + 8, sizeof(input_file) - 1);
        }
        else if (strncmp(argv[i], "--processes=", 12) == 0)
        {
            strncpy(processes_str, argv[i] + 12, sizeof(processes_str) - 1);
        }
        else if (strncmp(argv[i], "--quantum=", 10) == 0)
        {
            quantum = atoi(argv[i] + 10);
            if (quantum <= 0)
            {
                fprintf(stderr, "Error: Quantum must be a positive integer.\n");
                return 1;
            }
        }
        else
        {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            return 1;
        }
    }

    if (strlen(algorithm) == 0)
    {
        fprintf(stderr, "Error: No algorithm specified. Use --algorithm=<name>.\n");
        return 1;
    }

    if (strlen(input_file) == 0 && strlen(processes_str) == 0)
    {
        fprintf(stderr, "Error: No workload provided. Use --input or --processes.\n");
        return 1;
    }

    int num_processes = 0;
    Process *processes = load_processes(input_file, processes_str, &num_processes);
    if (processes == NULL)
    {
        return 1;
    }

    SchedulerState state = {
        .processes = processes,
        .num_processes = num_processes,
        .current_time = 0,
        .context_switches = 0,
        .gantt_head = NULL,
        .gantt_tail = NULL};

    int result = run_scheduler(&state, algorithm, quantum);

    gantt_free(&state);
    reset_processes(processes, num_processes);
    free(processes);
    return result == 0 ? 0 : 1;
}
