/*
 * file: process.c
 * Implementation of process management functions for CPU scheduling simulation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

// Additional functions for process management can be implemented here
Process *load_processes(const char *filename, int *num_processes)
{
    /*
     * Implementation to load processes from a file
     * This function reads process data from a specified file and returns an array of Process structs.
     * The number of processes loaded is stored in the variable pointed to by num_processes.
     */

    return NULL; // Placeholder return
}

void print_process(Process *proc)
{
    printf("Process %s:\n", proc->pid);
    printf("    Arrival Time:   %d\n", proc->arrival_time);
    printf("    Burst Time:     %d\n", proc->burst_time);
    printf("    Finish Time:     %d\n", proc->finish_time);

    int turnaround_time = proc->finish_time - proc->arrival_time;
    int waiting_time = turnaround_time - proc->burst_time;
    int response_time = proc->start_time - proc->arrival_time;

    printf("    Turnaround Time: %d - %d = %d\n",
           proc->finish_time, proc->arrival_time, turnaround_time);

    printf("    Waiting Time: %d - %d = %d\n",
           turnaround_time, proc->burst_time, waiting_time);

    printf("    Response Time: %d - %d = %d\n",
           proc->start_time, proc->arrival_time, response_time);
    printf("\n");
}