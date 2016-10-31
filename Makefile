CC=gcc

default: collatzl_c test

all: collatzl_c test

test: test_collatzl_c

test_collatzl_c:
	./collatzl_c.o 0 1000 1000 2 1 0 2 3 1 1

collatzl_c: collatzl.c glib.c
	$(CC) collatzl.c -o collatzl_c.o
