/*
 * file: utils.c
 * Global-use functions are stored here
 */

#include <stdio.h>
#include <string.h>

void log_process_start(int time, const char *pid)
{
    if (pid == NULL)
        pid = "UNKNOWN";
    printf("[%04d] START %s\n", time, pid);
}

void log_process_finish(int time, const char *pid)
{
    if (pid == NULL)
        pid = "UNKNOWN"; 
    printf("[%04d] FINISH %s\n", time, pid);
}

void log_context_switch(int time, const char *from_pid, const char *to_pid)
{
    if (from_pid == NULL || from_pid[0] == '\0')
        from_pid = "IDLE";
    if (to_pid == NULL || to_pid[0] == '\0')
        to_pid = "IDLE";
    printf("[%04d] CONTEXT SWITCH %s -> %s\n", time, from_pid, to_pid);
}

void log_idle_interval(int start_time, int end_time)
{
    if (end_time > start_time)
        printf("[%04d-%04d] IDLE\n", start_time, end_time);
}
