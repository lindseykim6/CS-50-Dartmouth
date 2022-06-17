# Makefile for 'Nugget' module
#
# Jeanmarcos Perez, October 2021


#OBJS = server.o client.o
OBJS = server.o
LIBS = ./common/common.a ./libcs50/libcs50-given.a ./support/support.a -lcurses -lm

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I./libcs50 -I./common -I./support

CC = gcc
MAKE = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: all clean test valgrind


#all: server client
all: server
# make -C support
# make -C common

# client:	$(OBJS) $(LIBS)
# 			$(CC) client.o $(CFLAGS)  $(LIBS) -o client 


server:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

# client:	$(OBJS) $(LIBS)
# 			$(CC) client.o $(CFLAGS)  $(LIBS) -o client 