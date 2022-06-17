# Makefile for common
CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I../libcs50
LIBS=common.a
OBJS= pagedir.o index.o word.o 

.PHONY: clean

common.a: $(OBJS)
	ar cr  $(LIBS) $(OBJS)

pagedir.o: pagedir.h
index.o: index.h

clean:
	rm -rf *.dSYM
	rm -f common.a
	rm -f *~ *.o