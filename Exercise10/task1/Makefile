CFLAGS = -std=gnu11 -Wall -Werror -Wextra -O2
LFLAGS = -pthread

.PHONY: all
all: task1

.PHONY: clean
clean:
	$(RM) task1 
    
task1: task1.c membench.c
	gcc $(CFLAGS) $^ -o task1 $(LFLAGS)