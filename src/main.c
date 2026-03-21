/*
 * file: main.c
 * CLI and main loop
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/gantt.h"
#include "../include/metrics.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <workload_file>\n", argv[0]);
        fprintf(stderr, "Example: %s workload.txt\n", argv[0]);
        return 1;
    }
    
    int num_processes = 0;
    Process *processes = load_processes(argv[1], &num_processes);

    SchedulerState state = {
        .processes = processes,
        .num_processes = num_processes,
        .current_time  = 0
    };

    printf("Loaded %d process(es):\n\n", state.num_processes);
    printf("%-10s %-15s %-10s\n", "PID", "Arrival", "Burst");
    printf("-----------------------------------\n");
    for (int i = 0; i < state.num_processes; i++) {
        printf("%-10s %-15d %-10d\n",
               state.processes[i].pid,
               state.processes[i].arrival_time,
               state.processes[i].burst_time);
    }

    free(processes);
    return 0;
}

