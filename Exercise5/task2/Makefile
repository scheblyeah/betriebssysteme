CC = gcc
CFLAGS = -std=gnu11 -Werror -Wall  -Wextra
LDFLAGS = -lrt -pthread


.PHONY: all clean

all: task2 task2b

clean:
	$(RM) task2 task2b

task2:
	$(CC) $(CFLAGS) task2.c -o task2 $(LDFLAGS)

task2b:
	$(CC) $(CFLAGS) task2b.c -o task2b $(LDFLAGS)



