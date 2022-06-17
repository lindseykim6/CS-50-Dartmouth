CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb
OBJS= counters.o ../lib/mem.o

.PHONY: test clean

all: counters.o

counters.o: counters.h

test: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) counterstest.c -o counterstest
	./counterstest

clean:
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f counterstest