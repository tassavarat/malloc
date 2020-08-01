# Malloc implementation

CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -ggdb3

TASK0 = naive_malloc
TASK1 = malloc
TASK2 = free
TASK3 = calloc
TASK4 = realloc

$(TASK4): $(TASK4).c $(TASK3).c $(TASK2).c $(TASK1).c 4-main.c
	$(CC) $(CFLAGS) -o $@  $^

$(TASK3): $(TASK3).c $(TASK2).c $(TASK1).c 3-main.c
	$(CC) $(CFLAGS) -o $@  $^

$(TASK2): $(TASK2).c $(TASK1).c 2-main.c
	$(CC) $(CFLAGS) -o $@  $^

$(TASK1): $(TASK2).c $(TASK1).c 1-main.c
	$(CC) $(CFLAGS) -o $@  $^

$(TASK0): $(TASK0).c 0-main.c
	$(CC) $(CFLAGS) -o $@  $^

.PHONY: clean, check, run

clean:
	rm $(TASK0) $(TASK1) $(TASK2) $(TASK3) $(TASK4)

run: $(TASK0)
	@valgrind --leak-check=full ./$^

check:
	betty *.c *.h
