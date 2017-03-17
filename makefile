# Makefile for automata
CC= gcc
CFLAGS= -I/usr/local/include
LFLAGS= -L/usr/local/lib -lgsl -lgslcblas -lm
DEPS = arrays.h c_automata.h
OBJS = c_automata.o arrays.o

.PHONY: all
all: test_benchmark.out test_automata.out test_pdf.out

test_benchmark.out: $(OBJS) test_benchmark.o
	$(CC) $(OBJS) test_benchmark.o  -o $@ $(LFLAGS)

test_automata.out: $(OBJS) test_automata.o
	$(CC) $(OBJS) test_automata.o -o $@ $(LFLAGS)
	
test_pdf.out : $(OBJS) test_pdf.o
	$(CC) $(OBJS) test_pdf.o -o $@ $(LFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean

# Clean up
clean:
	rm -f $(OBJS)
