/*
 * file: process.c
 * Implementation of process management functions for CPU scheduling simulation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process.h"

Process *load_processes(const char *filename, int *num_processes)
{

	sprintf(MAX_LINE, sizeof(MAX_LINE), "tests/%s", filename);

	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Error: Could not open file '%s'\n", filename);
		return NULL;
	}

	int starting_capacity = 4;          
	int count = 0;

	Process *processes = malloc(starting_capacity * sizeof(Process));
	if (processes == NULL) {
		fprintf(stderr, "Error: Memory allocation failed\n");
		fclose(file);
		return NULL;
	}

	char line[MAX_LINE];

	while (fgets(line, MAX_LINE, file) != NULL) {
		char pid[16];
		int at, bt;

		if (sscanf(line, "%15s %d %d", pid, &at, &bt) != 3) {
            fprintf(stderr, "Warning: Skipping malformed line: %s", line);
            continue;
        }

		if (count == starting_capacity) {
			starting_capacity *= 2;
			Process *temp = realloc(processes, starting_capacity * sizeof(Process));
			if (temp == NULL) {
				fprintf(stderr, "Error: Memory reallocation failed\n");
				free(processes);
				fclose(file);
				return NULL;
			}
			processes = temp;
		}

		strncpy(processes[count].pid, pid, sizeof(processes[count].pid) - 1);
		processes[count].pid[15]        = '\0';
		processes[count].arrival_time   = at;
		processes[count].burst_time     = bt;
		processes[count].remaining_time = bt;
		processes[count].start_time     = -1;
		processes[count].finish_time    = -1;
		processes[count].waiting_time   = 0;
		processes[count].priority       = 0;
		processes[count].time_in_queue  = 0;

		print_process(processes[count]);
		
		count++;
	}

	fclose(file);
	*num_processes = count;
	return processes;
}


void print_process(Process proc)
{
    printf("Process %s:\n", proc.pid);
    printf("    Arrival Time:   %d\n", proc.arrival_time);
    printf("    Burst Time:     %d\n", proc.burst_time);
    printf("    Finish Time:     %d\n", proc.finish_time);

    int turnaround_time = proc.finish_time - proc.arrival_time;
    int waiting_time = turnaround_time - proc.burst_time;
    int response_time = proc.start_time - proc.arrival_time;

    printf("    Turnaround Time: %d - %d = %d\n",
           proc.finish_time, proc.arrival_time, turnaround_time);

    printf("    Waiting Time: %d - %d = %d\n",
           turnaround_time, proc.burst_time, waiting_time);

    printf("    Response Time: %d - %d = %d\n",
           proc.start_time, proc.arrival_time, response_time);
    printf("\n");
}