CC=gcc

default: c

all: c gmp

test: test_collatzl_c test_collatzl_gmp

c: collatzl_c test_collatzl_c

gmp: collatzl_gmp test_collatzl_gmp


# In plain C
collatzl_c: collatzl.c glib.c
	$(CC) collatzl.c glib.c -o collatzl_c.o

test_collatzl_c:
	./collatzl_c.o 0 1000 1000 2 1 0 2 3 1 1

# with GMP
collatzl_gmp: collatzl_gmp.c glib.c
	$(CC) collatzl_gmp.c glib.c -lgmp -o collatzl_gmp.o

test_collatzl_gmp:
	./collatzl_gmp.o 0 1000 1000 2 1 0 2 3 1 1
