# Makefile for automata
CC = gcc
OBJS = automata.o test_automata.o arrays.o random.o

# Test run
test_automata.out: $(OBJS)
	$(CC) $(OBJS) -o test_automata.out
	
test_automata.o: test_automata.c
	$(CC) -c test_automata.c
	

# Automata Library
automata.o: automata.c
	$(CC) -c automata.c

# Dependencies
arrays.o: arrays.c
	$(CC) -c arrays.c
	
random.o: random.c
	$(CC) -c random.c


.PHONY: clean

# Clean up
clean:
	rm -f $(OBJS)