/*
 * file: process.c
 * Implementation of process management functions for CPU scheduling simulation.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

Process *load_processes(const char *filename, const char *processes_input, int *num_processes)
{
    int capacity = 4;
    int count = 0;
    Process *processes = malloc(capacity * sizeof(Process));
    if (!processes) return NULL;

    // CASE 1: Load from File, this is one is priority regardless if there's process or not
    if (filename != NULL && strlen(filename) > 0) {
        FILE *file = fopen(filename, "r");
        if (!file) {
            free(processes);
            return NULL;
        }
        char line[MAX_LINE];
        while (fgets(line, MAX_LINE, file)) {
            if (count >= capacity) {
                capacity *= 2;
                processes = realloc(processes, capacity * sizeof(Process));
            }
            if (sscanf(line, "%15s %d %d", processes[count].pid, 
                       &processes[count].arrival_time, 
                       &processes[count].burst_time) == 3) {
                processes[count].remaining_time = processes[count].burst_time;
                processes[count].start_time = -1;
                count++;
            }
        }
        fclose(file);
    } 
    // CASE 2: Load from String (A:0:240,B:10:180)
    else if (processes_input != NULL && strlen(processes_input) > 0) {
        char *input_copy = strdup(processes_input);
        char *saveptr1, *saveptr2;
        char *token = strtok_r(input_copy, ",", &saveptr1);
        
        while (token != NULL) {
            if (count >= capacity) {
                capacity *= 2;
                processes = realloc(processes, capacity * sizeof(Process));
            }

            char *p_id = strtok_r(token, ":", &saveptr2);
            char *p_at = strtok_r(NULL, ":", &saveptr2);
            char *p_bt = strtok_r(NULL, ":", &saveptr2);

            if (p_id && p_at && p_bt) {
                // Correct way to copy to the PID array
                strncpy(processes[count].pid, p_id, 15);
                processes[count].pid[15] = '\0'; 
                
                processes[count].arrival_time = atoi(p_at);
                processes[count].burst_time = atoi(p_bt);
                processes[count].remaining_time = processes[count].burst_time;
                processes[count].start_time = -1;
                processes[count].finish_time = -1;
                processes[count].waiting_time = 0;
                processes[count].priority = 0;
                processes[count].time_in_queue = 0;
                count++;
            }
            token = strtok_r(NULL, ",", &saveptr1);
        }
        free(input_copy);
    }

    *num_processes = count;
    return processes;
}

void reset_processes(Process *processes, int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        processes[i].remaining_time  = processes[i].burst_time;
        processes[i].start_time      = -1;
        processes[i].finish_time     = -1;
        processes[i].waiting_time    = 0;
        processes[i].turnaround_time = 0;
        processes[i].response_time   = 0;
        processes[i].priority        = 0;
        processes[i].time_in_queue   = 0;
    }
}

void sort_processes(Process *processes, int num_processes)
{
    // Sort processes according to arrival time
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = 0; j < num_processes - i - 1; j++) {
            if (processes[j].arrival_time > processes[j+1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }
        }
    }
}

void print_process(Process process)
{
	printf("Process %s:\n", process.pid);
	printf("    Arrival Time:   %d\n", process.arrival_time);
	printf("    Burst Time:     %d\n", process.burst_time);
	printf("    Finish Time:     %d\n", process.finish_time);

	int turnaround_time = process.finish_time - process.arrival_time;
	int waiting_time = turnaround_time - process.burst_time;
	int response_time = process.start_time - process.arrival_time;

	printf("    Turnaround Time: %d - %d = %d\n",
		   process.finish_time, process.arrival_time, turnaround_time);

	printf("    Waiting Time: %d - %d = %d\n",
		   turnaround_time, process.burst_time, waiting_time);

	printf("    Response Time: %d - %d = %d\n",
		   process.start_time, process.arrival_time, response_time);
	printf("\n");
}