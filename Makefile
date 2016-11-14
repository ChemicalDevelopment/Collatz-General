CC=gcc

CSRC=collatzl.c glib.c
GMPSRC=collatzl_gmp.c glib.c
PYSRC=collatzl.py glib.py

default: all_c

all: all_c all_gmp all_python
test: test_c test_gmp test_python

all_c: c test_c
all_python: python test_python
all_gmp: gmp test_gmp

# In plain Cx
c: $(CSRC)
	$(CC) $(CSRC) -o collatzl_c.o

# In python
python: $(PYSRC)
	echo "Python isn't compiled!"

# with GMP
gmp: $(GMPSRC)
	$(CC) $(GMPSRC) -lgmp -Ofast -o collatzl_gmp.o

test_c:
	./collatzl_c.o 0 25000 1000 2 1 0 2 3 1 1

test_python:
	./collatzl.py 0 25000 1000 2 1 0 2 3 1 1

test_gmp:
	./collatzl_gmp.o 0 25000 1000 2 1 0 2 3 1 1

