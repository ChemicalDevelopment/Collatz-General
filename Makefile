CC=gcc

default: test_c

all: all_c all_gmp

test: test_c test_gmp

all_c: c test_c

all_gmp: gmp test_gmp


# In plain C
c: collatzl.c glib.c
	$(CC) collatzl.c glib.c -o collatzl_c.o

test_c:
	./collatzl_c.o 0 1000 1000 2 1 0 2 3 1 1

# with GMP
gmp: collatzl_gmp.c glib.c
	$(CC) collatzl_gmp.c glib.c -lgmp -o collatzl_gmp.o

test_gmp:
	./collatzl_gmp.o 0 1000 1000 2 1 0 2 3 1 1
