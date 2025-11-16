CC = gcc
CFLAGS = -I./include -Wall
SRCS = src/main.c src/architecture.c src/memory.c src/terminal.c src/trap.c
OBJS = $(SRCS:.c=.o)
TARGET = vm

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)