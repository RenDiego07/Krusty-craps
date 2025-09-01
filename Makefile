CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pthread
TARGET = krusty_crabs
SRCDIR = .
ORDERDIR = order
DISPENSERDIR = dispenser
QUEUEDIR = queue

# Object files
OBJS = main.o order/order.o dispenser/dispenser.o queue/queue.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c struct.h dispenser/dispenser.h queue/queue.h
	$(CC) $(CFLAGS) -c main.c

order/order.o: order/order.c order/order.h struct.h
	$(CC) $(CFLAGS) -c order/order.c -o order/order.o

dispenser/dispenser.o: dispenser/dispenser.c dispenser/dispenser.h struct.h queue/queue.h
	$(CC) $(CFLAGS) -c dispenser/dispenser.c -o dispenser/dispenser.o

queue/queue.o: queue/queue.c queue/queue.h struct.h
	$(CC) $(CFLAGS) -c queue/queue.c -o queue/queue.o

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean