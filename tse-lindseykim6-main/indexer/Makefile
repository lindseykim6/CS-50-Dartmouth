# Makefile for  indexer
CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I ../libcs50 -I../common
LIBS=../common/pagedir.c ../common/word.c ../common/index.c ../libcs50/libcs50-given.a
OBJS= indexer.o 

.PHONY: test valgrind clean

all: indexer test

indexer: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@


compiletest: indextest.o
	$(CC) $(CFLAGS) indextest.o $(LIBS) -o $@ 

test: indexer compiletest
	bash -v ./testing.sh &> testing.out

indexer.o: ../common/pagedir.h ../common/index.h ../libcs50/file.h ../libcs50/webpage.h ../common/word.h

indextest.o: ../common/index.h

clean:
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f indexer
	rm -f index
	rm -f test
	rm -f compiletest
	rm -rf ../data
	