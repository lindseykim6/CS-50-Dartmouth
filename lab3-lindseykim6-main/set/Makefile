CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb
OBJS= set.o ../lib/mem.o

.PHONY: test clean

all: set.o

set.o: set.h

test: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) settest.c -o settest
	./settest

clean:
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f settest