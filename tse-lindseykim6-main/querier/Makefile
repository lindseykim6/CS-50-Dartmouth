# Makefile for  indexer
CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I ../libcs50 -I../common
LIBS=../common/pagedir.c ../common/word.c ../common/index.c ../libcs50/libcs50-given.a
OBJS= querier.o 

.PHONY: test valgrind clean

all: querier test

querier: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

test: querier
	bash -v ./testing.sh &> testing.out

querier.o: ../common/pagedir.h ../common/index.h ../libcs50/file.h ../libcs50/webpage.h ../common/word.h

clean:
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f querier
	rm -f foo
	rm -f fooTwo