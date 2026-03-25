# MLFQ DESIGN JUSTIFICATION

## Overview

Our Multi-Level Feedback Queue (MLFQ) uses **3 priority levels using queues** with the following configurations:

| Queue   | Time Quantum | Allotment | Role                                          |
| ------- | ------------ | --------- | --------------------------------------------- |
| Q<sub>0 | 10           | 50        | For small jobs, highest priority              |
| Q<sub>1 | 30           | 150       | For medium jobs                               |
| Q<sub>2 | FCFS         | ♾️        | For large, long-running jobs, lowest priority |

**Boost period = 200**

---

## Design Decisions

### Number of Queues: 3

A three level priority queue is needed for three cases:

- **Short Jobs** complete entirely within Q<sub>0</sub>'s time.
- **Medium Jobs** complete in Q<sub>1</sub> or some parts in Q<sub>2</sub>.
- **Long Jobs** will be at thje lowest priority with FCFS scheduling.

<br>
A three-level queue is often recommended and used for optimal performance and balance between responsiveness and overhead, unlike those having more or less than three levels. The typical reason for this is because of these reasons:

- A two-level queue is not usually enough for job separations, therefore limiting the scheduler's ability to differentiate between types of tasks.
- Increasing a level to four or more levels could cause an increased overheasd. This means that the CPU tends to spend more time _deciding_ what tasks to run instead of actually _running_ them.

### Quantum Time per Level

- Q<sub>0</sub>, q=10: This quantum time keeps the response time low for accomodating small jobs, making it perfect for exposing most CPU-intensive porcesses.
- Q<sub>1</sub>, q=30: This quantum time is used for medium jobs to mainly reduce overhead caused by context-switches.
- Q<sub>2</sub>, use FCFS: Once a job is considered long-running, that job will be then sent to the lowest priority. An FCFS scheduler is then used for handling those long-running jobs for maximum throughput.

### Allotment Values

- Q<sub>0</sub>, allotment time=50: A process only gets 5xq<sub>0</sub>=**50** units before demoting to a lower priority. This basically allows a job, as long as the process fits to 40 units, to complete in the Q<sub>0</sub> time range.
- Q<sub>1</sub>, allotment time=150: A 5xq<sub>1</sub>=**150** units is given to those processes that needs more time to complete. Any more time than this could end up having more long-running jobs in the same queue, causing response times to fluctuate often.
- Q<sub>2</sub>, allotment time=♾️: An FCFS scheduler should be used for managing long-running jobs in the long run for fairness. Limiting allotment could mean a higher overhead.

### Boost Period: 200

- The chosen boost is for all processes to recieve at least one boost, so that no process could starve for more than 200 units.

---

## Why MLFQ Should NOT Read Burst Time

The main purpose of a Multi-Level Feedback Queue (MLFQ) is **adaptive scheduling**.

- Both **SJF/STCF** usually requires burst times the Operating System does not have knowledge at for general workloads.
- A **MLFQ** is them implemented just from observing CPU consumption of programs.
  - Any job that completes within the Q<sub>0</sub> time range is short, therefore having a good response time.
  - Any job that does not fit in Q<sub>0</sub> should be demoted and have lower priority than Q<sub>0</sub>.
  - A priority boost is used to prevent accumulated starvation of long-running jobs in lower-level priorities.

In the implementation, the `schedule_mlfq()` function never reads at `p->burst_time` but rather reads at `p->remaining_time` to detect completion once `p->remaining_time` reaches zero. And demotions are only decided based on `p->time_in_queue` and `mq->allotment`.

---

### Testing: Activity Workload

```
Algorithm | Avg TT | Avg WT | Avg RT | Context Switches
----------|--------|--------|--------|------------------
FCFS      |  515.0 |  359.0 |  359.0 |        4
SJF       |  461.0 |  305.0 |  305.0 |        4
STCF      |  393.0 |  237.0 |   15.0 |        7
RR(q=30)  |  627.0 |  471.0 |   43.0 |       25
MLFQ      |  649.0 |  493.0 |   17.0 |       63
```

**Observations:**

- MLFQ has achieved an average response time of 17s. That is nearly as low as STCF's average response time of 15s. This is for short jobs that stay in Q<sub>0</sub> and are quickly scheduled on arrival.
- MLFQ has the largest average turnaround time of 649s. This is because MLFQ does not have the knowledge about burst times, which some schedulers take advantage, and MLFQ must look for job length through observed job behavior, `p->remaining_time`.
- MLFQ has an average response time (17s) that is faster than Round Robin's average response time (43s). This shows that priority-based scheduling is sometimes better than that of Round Robin's fairness. Though that depends on specific workload requirements.
- Context switches of MLFQ (63) are often higher than Round Robin (25) because of allotment and priority boosts.

---

## Other MLFQ Mechanisms

- Alloment is read to be cumulatively tracked across all time slices whithim the same queue level. This is by using `time_in_queue` from the process structure.
- `time_in_queue` resets to 0 only when in demotion or boosted phase.
