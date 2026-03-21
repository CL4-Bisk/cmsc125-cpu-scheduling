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

    if(schedule_fcfs(&state)){
        printf("Successful: FCFS");
    } else {
        printf("Unsuccessful: FCFS");
    }
    if(schedule_sjf(&state)){
        printf("Successful: FCFS");
    } else {
        printf("Unsuccessful: FCFS");
    }
    if(schedule_stcf(&state)){
        printf("Successful: FCFS");
    } else {
        printf("Unsuccessful: FCFS");
    }
    if(schedule_rr(&state, DEFAULT_QUANTUM)){
        printf("Successful: FCFS");
    } else {
        printf("Unsuccessful: FCFS");
    }

    free(processes);
    return 0;
}

