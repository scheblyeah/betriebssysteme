CFLAGS = -std=gnu11 -Wall -Werror -Wextra -O2
LFLAGS = -pthread

.PHONY: all
all: task3

.PHONY: clean
clean:
	$(RM) task3 
    
task3: task3.c membench.c
	gcc $(CFLAGS) $^ -o task3 $(LFLAGS)