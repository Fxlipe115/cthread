CC=gcc
CFLAGS=-Wall -I../include -std=gnu99 -m32 -ggdb3 -L../lib -lcthread
BIN=../bin/insert.o ../bin/cthread.o ../bin/support.o
LIB=../lib/libcthread.a

all:

semaphorestest: semaphorestest.c $(LIB)
	$(CC) -o $@ $< $(CFLAGS)

twinkleStar: twinkleStar.c $(LIB)
	$(CC) -o $@ $< $(CFLAGS)

testeCompleto: testeCompleto.c $(LIB)
	$(CC) -o $@ $< $(CFLAGS)

testPriority: testPriority.c $(LIB)
	$(CC) -o $@ $< $(CFLAGS)

