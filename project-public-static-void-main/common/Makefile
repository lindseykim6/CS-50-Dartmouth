# Makefile for common
CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../support
LIBS=common.a
OBJS= game.o grid.o player_struct.o roster.o

.PHONY: clean

common.a: $(OBJS)
	ar cr  $(LIBS) $(OBJS)

game.o: game.h
player_struct.o: player_struct.h
grid.o: grid.h
roster.o: roster.h

clean: 
	rm -rf *.dSYM
	rm -f common.a
	rm -f *~ *.o 