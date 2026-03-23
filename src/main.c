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
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <workload_file>\n", argv[0]);
        fprintf(stderr, "Example: %s workload.txt\n", argv[0]);
        return 1;
    }
    
    int num_processes = 0;
    Process *processes = load_processes(argv[1], &num_processes);
    if (processes == NULL) {
        return 1;
    }

    SchedulerState state = {
        .processes = processes,
        .num_processes = num_processes,
        .current_time  = 0
    };

    if(schedule_fcfs(&state) == 0){
        printf("Successful: FCFS");
    } else {
        printf("Unsuccessful: FCFS");
    }
    reset_processes(processes, num_processes);
    if(schedule_sjf(&state) == 0){
        printf("Successful: SJF");
    } else {
        printf("Unsuccessful: SJF");
    }
    reset_processes(processes, num_processes);
    if(schedule_stcf(&state) == 0){
        printf("Successful: STCF");
    } else {
        printf("Unsuccessful: STCF");
    }
    reset_processes(processes, num_processes);
    if(schedule_rr(&state, DEFAULT_QUANTUM) == 0){
        printf("Successful: RR");
    } else {
        printf("Unsuccessful: RR");
    }
    reset_processes(processes, num_processes);
    free(processes);
    printf("Run everything succesfully.");
    return 0;
}

