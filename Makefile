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
       src/utils.c 

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)