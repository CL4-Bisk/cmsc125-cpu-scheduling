/*
 * file: utils.c
 * Global-use functions are stored here
 */
#include "../include/process.h"

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