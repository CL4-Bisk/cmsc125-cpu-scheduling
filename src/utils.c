/*
 * file: utils.c
 * Global-use functions are stored here
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

char *log_entries[256];
int counter = 0;

void log_process_start(int time, const char *pid) {
    if (pid == NULL) 
        pid = "UNKNOWN";
    asprintf(&log_entries[counter], "[%04d] START %s\n", time, pid);
    counter++;
}

void log_process_finish(int time, const char *pid) {
    if (pid == NULL)    
        pid = "UNKNOWN";
    asprintf(&log_entries[counter], "[%04d] FINISH %s\n", time, pid);
    counter++;
}

void log_context_switch(int time, const char *from_pid, const char *to_pid) {
    if (from_pid == NULL || from_pid[0] == '\0') 
        from_pid = "IDLE";
    if (to_pid == NULL || to_pid[0] == '\0') 
        to_pid = "IDLE";
    asprintf(&log_entries[counter], "[%04d] CONTEXT SWITCH %s -> %s\n", time, from_pid, to_pid);
    counter++;
}

void log_idle_interval(int start_time, int end_time) {
    if (end_time > start_time) { 
        asprintf(&log_entries[counter], "[%04d-%04d] IDLE\n", start_time, end_time);
        counter++;
    }
}

void print_logs() {
    asprintf(&log_entries[counter], "\0");
    for (int i = 0; i < counter; i++) {
        if (log_entries[i] != NULL) {
            printf("%s", log_entries[i]);
            free(log_entries[i]);          
            log_entries[i] = NULL;
        }
    }
    counter = 0;
}