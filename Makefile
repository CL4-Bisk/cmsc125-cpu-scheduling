CC = gcc
CFLAGS = -Wall -Wextra -std=c11
# Added flag for command recall if code does not work try installing:
# sudo apt-get update && sudo apt-get install libreadline-dev


TARGET = schedsim
SRCS = 

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)