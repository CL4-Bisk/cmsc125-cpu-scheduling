CC = gcc
CFLAGS = -Wall -Wextra -std=c11
# Added flag for command recall if code does not work try installing:
# sudo apt-get update && sudo apt-get install libreadline-dev


TARGET = schedsim
SRCS = src/main.c      \
       src/fcfs.c      \
       src/sjf.c       \
       src/stcf.c      \
       src/rr.c        \
       src/mlfq.c      \
       src/gantt.c     \
       src/metrics.c   \
       src/process.c   \
       src/utils.c 	   \
	   src/scheduler.c \
	   src/sched_utils.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

# Run targets for different algorithms and workloads
fcfs_input-1: $(TARGET)
	./$(TARGET) --algorithm=FCFS --input=tests/workload1.txt

fcfs_input-2: $(TARGET)
	./$(TARGET) --algorithm=FCFS --input=tests/workload2.txt

fcfs_input-3: $(TARGET)
	./$(TARGET) --algorithm=FCFS --input=tests/workload3.txt

sjf_input-1: $(TARGET)
	./$(TARGET) --algorithm=SJF --input=tests/workload1.txt

sjf_input-2: $(TARGET)
	./$(TARGET) --algorithm=SJF --input=tests/workload2.txt

sjf_input-3: $(TARGET)
	./$(TARGET) --algorithm=SJF --input=tests/workload3.txt

stcf_input-1: $(TARGET)
	./$(TARGET) --algorithm=STCF --input=tests/workload1.txt

stcf_input-2: $(TARGET)
	./$(TARGET) --algorithm=STCF --input=tests/workload2.txt

stcf_input-3: $(TARGET)
	./$(TARGET) --algorithm=STCF --input=tests/workload3.txt

rr_input-1: $(TARGET)
	./$(TARGET) --algorithm=RR --input=tests/workload1.txt

rr_input-2: $(TARGET)
	./$(TARGET) --algorithm=RR --input=tests/workload2.txt

rr_input-3: $(TARGET)
	./$(TARGET) --algorithm=RR --input=tests/workload3.txt

mlfq_input-1: $(TARGET)
	./$(TARGET) --algorithm=MLFQ --input=tests/workload1.txt

mlfq_input-2: $(TARGET)
	./$(TARGET) --algorithm=MLFQ --input=tests/workload2.txt

mlfq_input-3: $(TARGET)
	./$(TARGET) --algorithm=MLFQ --input=tests/workload3.txt