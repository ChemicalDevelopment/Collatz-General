# CollatzL

Tests modified collatz conjectures (including the original).

# Installation

Make sure you have `gcc`.

Now, clone this repo with:

`git clone https://github.com/ChemicalDevelopment/CollatzL.git`

change to that directory:

`cd CollatzL`

compile and test the code:

`make`

There should be no errors, and the last line should read:

`Hash: 1418640`

If not, please open up an issue.

# Usage

This program is used to verify, or counter Collatz-like problems (include original Collatz).

The original collatz conjecture is:

```
define f(x):
if x is even, f(x) = x / 2
if x is odd, f(x) = 3x+1
```

And the question is: Does iteration of f(x) `f(f(f(...(x)))))` eventually reach a cycle, or keep expanding?

We don't know, but all numbers <= 4*10^18 do reach a cycle.

An example of a collatz like function would be:

```
define f(x):
if x is 0 mod 3, f(x) = x / 3
if x is 1 mod 3, f(x) = 2 * x + 1
if x is 2 mod 3, f(x) = 5 * x + 4 
```

And again, the question is: Does iteration of f(x) eventually reach a cycle?

To test the original collatz conjecture, run

`./collatzl_c.o $MIN $RANGE $MAX_TRIALS 2 1 0 2 3 1 1`

It starts at MIN, and runs RANGE numbers each at a max of MAX_TRIALS

The `2` after that tells us we take the number mod 2 (even or odd), and the `1 0 2` tells us to multiply by `1`, add `0`, and divide by `2`

the `3 1 1` tells us to multiply by `3`, add `1`, and divide by `1`, if it is 1 mod 2

`./collatzl_c.o 0 1000 1000 2 1 0 2 3 1 1` runs quite quickly, and verifies they all work

To test our other example, the command would be:

`./collatzl_c.o 0 1000 1000 3 1 0 3 2 1 1 5 4 1`

## GMP

There is also a version for GMP, which you can build and test with:

`make gmp`

The interface is the same as the normal, but the executable is `collatzl_gmp.o` instead of `collatzl_c.o`

To run the default command, use:

`./collatzl_gmp.o 0 1000 1000 2 1 0 2 3 1 1`

And our example would be:

`./collatzl_gmp.o 0 1000 1000 3 1 0 3 2 1 1 5 4 1`
