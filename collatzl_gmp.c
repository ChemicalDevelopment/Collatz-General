/*

Run like

./collatz_gmp.o <min> <max> <max_trials> <residue class> <(mul, add, div) . . .>

To test the original collatz conjecture, run:

./collatz_gmp.o 0 1000000 1000 2 1 0 2 3 1 1

(default is testing 0-1000 inclusive, up to 1000 trials)

The first `2` tells us we take out function % 2, then we read the next 3 values entered (1, 0, 2). Lets call them a0, b0, c0 respectively. The next 3 values (3, 1, 1) well call a1, b1, c1

if x % 2 = 0, f(x) = (a0*x+b0)/c0
if x % 2 = 1, f(x) = (a1*x+b1)/c1

Which simplifies to:

if x % 2 = 0, f(x) = x/2
if x % 2 = 1, f(x) = 3x+1

We want to see if every number loops back, which is currently unproven.

You can call it with as RESIDUECLASS number of groups of 3 arguments, and it will respond like:

if x % RESIDUECLASS = 0, f(x) = (a0*x+b0)/c0
if x % RESIDUECLASS = 1, f(x) = (a1*x+b1)/c1
. . . 
if x % RESIDUECLASS = (RESIDUECLASS-1), f(x) = (a(RESIDUECLASS-1)*x+b(RESIDUECLASS-1))/c(RESIDUECLASS-1)

*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <gmp.h>

// our library
#include "glib.h"

// past evaluations of f(x)
mpz_t *history;

// temporary value to use
mpz_t tmp, r_x;

unsigned int res;

// function used for iteration
void f(mpz_t x) {
	res = mpz_fdiv_r_ui(tmp, x, residue_class);
    mpz_mul_ui(x, x, residue_mul[res]);
	mpz_add_ui(x, x, residue_add[res]);
	mpz_div_ui(x, x, residue_div[res]);
}

// main
int main(int argc, char *argv[]) {
    read_args(argv);
	//keep track of time
    clock_t start, end;
    // init to keep track of what repeated.
    long long x, i, trials;

    // init our mpz vars
	mpz_inits(r_x, tmp);
    // initialize enough mpz for a history
    history = (mpz_t *)malloc(sizeof(mpz_t) * MAX_TRIALS);
	for (i = 0; i < MAX_TRIALS; ++i) mpz_init(history[i]);
    
    // print info and start clock
    print_start_info();
    start = clock();

    // loop through their min and max, checking each
    for (x = MIN; x < MIN + RANGE; ++x) {
        trials = 0;
        // set r_x to our current var
        mpz_set_ui(r_x, x);
        current_repeated = false;
        // keep going
        while (trials < MAX_TRIALS) {
            // keep setting the history
			mpz_set(history[trials], r_x);
            // this computes f(r_x) and sets r_x to that
            f(r_x);
            // add to trials
            trials++;
            // we check if it repeats
            for (i = 0; i < trials; ++i) {
                if (mpz_cmp(r_x, history[i]) == 0) {
                    goto skipahead;
                }
            }
            // we increment how many trials we've done
        }
        //label this so we can skip out
        skipahead:current_repeated = true;
        all_repeated = all_repeated && current_repeated;
        one_repeated = one_repeated || current_repeated;
        if (!current_repeated) {
            printf("%lld does not repeat\n", x);
        } else {
            total_repeats++;
        }
        // add however many we just did
        total_trials += trials;
    }

    end = clock();
    //compute elapsed microseconds (1 millionth of a second)
    elapsed_micros = (end - start);

    // print out more info
    print_end_info(elapsed_micros, total_trials);

    return 0;
}
