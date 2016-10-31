CC=gcc

default: collatzl_c test_collatzl_c

all: collatzl_c collatzl_gmp test

test: test_collatzl_c test_collatzl_gmp

test_collatzl_c:
	./collatzl_c.o 0 1000 1000 2 1 0 2 3 1 1

test_collatzl_gmp:
	./collatzl_gmp.o 0 1000 1000 2 1 0 2 3 1 1

collatzl_c: collatzl.c glib.c
	$(CC) collatzl.c glib.c -o collatzl_c.o

collatzl_gmp: collatzl_gmp.c glib.c
	$(CC) collatzl_gmp.c glib.c -lgmp -o collatzl_gmp.o
