# Makefile for automata
CC= gcc
CFLAGS= -I.
DEPS = arrays.h c_automata.h random.h
OBJS = c_automata.o arrays.o random.o

.PHONY: all
all: test_benchmark.out test_automata.out

test_benchmark.out: $(OBJS) test_benchmark.o
	$(CC) $(OBJS) test_benchmark.o  -o $@ $(CFLAGS)

test_automata.out: $(OBJS) test_automata.o
	$(CC) $(OBJS) test_automata.o -o $@ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean

# Clean up
clean:
	rm -f $(OBJS)
