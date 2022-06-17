CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb
OBJS= hashtable.o ../lib/mem.o ../set/set.o ../hashtable/hash.o

.PHONY: test clean

all: hashtable.o ../set/set.o

hashtable.o: hashtable.h

test: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) hashtabletest.c -o hashtabletest 
	./hashtabletest

clean:
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f hashtabletest