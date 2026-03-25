/*
 * file: gantt.c
 * Gantt chart functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/gantt.h"

void gantt_add_entry(SchedulerState *state, const char *pid, int start, int end)
{
    GanttEntry *entry = malloc(sizeof(GanttEntry));
    if (entry == NULL) {
        fprintf(stderr, "Error: Gantt entry allocation failed\n");
        return;
    }

    strncpy(entry->pid, pid, sizeof(entry->pid) - 1);
    entry->pid[15]    = '\0';
    entry->start_time = start;
    entry->end_time   = end;
    entry->next       = NULL;

    if (state->gantt_tail == NULL) {
        state->gantt_head = entry;
        state->gantt_tail = entry;
    } else {
        state->gantt_tail->next = entry;
        state->gantt_tail       = entry;
    }
}

void gantt_add_or_extend(SchedulerState *state, const char *pid, int start, int end)
{
    if (state->gantt_tail != NULL && strcmp(state->gantt_tail->pid, pid) == 0)
    {
        state->gantt_tail->end_time = end;
        return;
    }
    gantt_add_entry(state, pid, start, end);
}

void gantt_print(SchedulerState *state)
{
    if (state->gantt_head == NULL) {
        printf("No Gantt entries to display.\n");
        return;
    }

    printf("\n=== Gantt Chart ===\n\n");

    printf("|");
    for (GanttEntry *e = state->gantt_head; e != NULL; e = e->next) {
        int width = e->end_time - e->start_time;
        if (width < 4) width = 4;
        int llen  = strlen(e->pid);
        int pad   = width - llen;
        int padl  = pad / 2;
        int padr  = pad - padl;

        for (int i = 0; i < padl; i++) printf("-");
        printf("%s", e->pid);
        for (int i = 0; i < padr; i++) printf("-");
        printf("|");
    }
    printf("\n");

    printf("%d", state->gantt_head->start_time);
    for (GanttEntry *e = state->gantt_head; e != NULL; e = e->next) {
        int width = e->end_time - e->start_time;
        if (width < 4) width = 4;
        printf("%*d", width, e->end_time);
    }
    printf("\n\n");
}

void gantt_free(SchedulerState *state)
{
    GanttEntry *curr = state->gantt_head;
    while (curr != NULL) {
        GanttEntry *next = curr->next;
        free(curr);
        curr = next;
    }
    state->gantt_head = NULL;
    state->gantt_tail = NULL;
}