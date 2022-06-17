# Makefile for  crawler
CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I ../libcs50 -I../common
LIBS=../common/pagedir.c ../libcs50/libcs50-given.a
OBJS= crawler.o

.PHONY: test valgrind clean

all: crawler

crawler: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@
	

test: crawler
	bash -v ./testing.sh &> testing.out

crawler.o: ../common/pagedir.h ../libcs50/bag.h ../libcs50/hashtable.h ../libcs50/mem.h ../libcs50/webpage.h


myvalgrind: crawler
	myvalgrind ./crawler

clean:
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f crawler