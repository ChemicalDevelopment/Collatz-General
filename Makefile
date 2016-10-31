CC=gcc

all: collatzl_c

collatzl_c: collatzl.c
	$(CC) collatzl.c -o collatzl_c.o
