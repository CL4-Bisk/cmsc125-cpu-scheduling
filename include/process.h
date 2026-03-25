#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES   100
#define MAX_LINE        256

typedef struct {
    char pid[16];           // Process identifier

    int arrival_time;       // When process arrives
    int burst_time;         // Total CPU time needed
    int remaining_time;     // For preemptive algorithms    
    int start_time;         // When first executed (for RT)
    int finish_time;        // When completed (for TT)
    int waiting_time;       // Time spent waiting
    int turnaround_time;   
    int response_time; 

    int priority;           // For MLFQ
    int time_in_queue;      // For MLFQ allotment tracking
} Process;

Process *load_processes(const char *filename, const char *processes_input, int *num_processes);
void reset_processes(Process *processes, int num_processes);
void sort_processes(Process *processes, int num_processes);
void print_process(Process process);

#endif