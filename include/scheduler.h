#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

#define DEFAULT_QUANTUM 30

typedef struct
{
    int level;        // Queue priority level (0 = highest)
    int time_quantum; // Time slice for this queue (-1 for FCFS)
    int allotment;    // Max time before demotion (-1 for infinite)
    Process *queue;   // Array or linked list of processes
    int size;         // Current queue size
} MLFQQueue;

typedef struct
{
    MLFQQueue *queues; // Array of queues
    int num_queues;    // Number of priority levels
    int boost_period;  // Period for priority boost (S)
    int last_boost;    // Last boost time
} MLFQScheduler;

// typedef enum
// {
//     EVENT_ARRIVAL,
//     EVENT_COMPLETION,
//     EVENT_QUANTUM_EXPIRE,
//     EVENT_PRIORITY_BOOST
// } EventType;

// typedef struct Event
// {
//     int time;
//     EventType type;
//     Process *process;
//     struct Event *next;
// } Event;

typedef struct
{
    Process **processes; // Array of pointers to processes
    int size;            // Number of processes in the queue
    int capacity;        // Maximum capacity of the queue
} ProcessQueue;

typedef struct
{
    int *data;          // Placeholder for any additional data needed by schedulers
    int head;
    int tail;
    int size;
    int capacity;
} IntQueue;


typedef struct GanttEntry
{
    char pid[16];            // Process identifier
    int start_time;          // When first executed
    int end_time;            // When completed
    struct GanttEntry *next; // Array or linked list of Gantt entries
} GanttEntry;

typedef struct
{
    Process *processes; // Array of all processes
    int num_processes;  // Number of processes
    int current_time;   // Current simulation time
    GanttEntry *gantt_head;
    GanttEntry *gantt_tail;
    int context_switches; // Total number of context switches
} SchedulerState;

// Return 0 on success, -1 on error (command line etiquette)
int schedule_fcfs(SchedulerState *state);
int schedule_sjf(SchedulerState *state);
int schedule_stcf(SchedulerState *state);
int schedule_rr(SchedulerState *state, int quantum);
int schedule_mlfq(SchedulerState *state, MLFQScheduler *mlfq_config);

#endif