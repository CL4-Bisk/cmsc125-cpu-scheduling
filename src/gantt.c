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

        int total_time = state->gantt_tail->end_time;

        int scale = 1;
        if (total_time > MAX_CHART_WIDTH)
            scale = (total_time + MAX_CHART_WIDTH - 1) / MAX_CHART_WIDTH;

        printf("\n=== Gantt Chart ===\n");
        if (scale > 1)
            printf("(Each character = %d time units)\n", scale);
        printf("\n");

        printf("|");
        for (GanttEntry *entry = state->gantt_head; entry != NULL; entry = entry->next) {
            int duration = entry->end_time - entry->start_time;
            int width    = (duration + scale - 1) / scale;  
            if (width < 1) width = 1;

            int llen = strlen(entry->pid);

            if (width <= llen) {
                for (int i = 0; i < width; i++) printf("-");
            } else {
                int pad  = width - llen;
                int padl = pad / 2;
                int padr = pad - padl;
                for (int i = 0; i < padl; i++) printf("-");
                printf("%s", entry->pid);
                for (int i = 0; i < padr; i++) printf("-");
            }
            printf("|");
        }
        printf("\n");

        int num_entries = 0;
        for (GanttEntry *entry = state->gantt_head; entry != NULL; entry = entry->next) {
            num_entries++;
        }
        int *cols  = malloc((num_entries + 1) * sizeof(int));
        int *times = malloc((num_entries + 1) * sizeof(int));

        int col = 1; /* start after the opening | */
        int idx = 0;

        /* First marker at column 0 */
        cols[idx]  = 0;
        times[idx] = state->gantt_head->start_time;
        idx++;

        for (GanttEntry *entry = state->gantt_head; entry != NULL; entry = entry->next) {
            int duration = entry->end_time - entry->start_time;
            int width    = (duration + scale - 1) / scale;
            if (width < 1) width = 1;
            col += width + 1; 
            cols[idx]  = col - 1; 
            times[idx] = entry->end_time;
            idx++;
        }

        int cur_col = 0;
        for (int i = 0; i < idx; i++) {
            int num_width = snprintf(NULL, 0, "%d", times[i]);
            int target    = cols[i];
            int spaces    = target - cur_col;
            if (spaces < 0) spaces = 0;
            printf("%*s%d", spaces, "", times[i]);
            cur_col = target + num_width;
        }
        printf("\n\n");

        free(cols);
        free(times);
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