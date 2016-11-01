/*

Run like

./collatz_c.o <min> <max> <max_trials> <residue class> <(mul, add, div) . . .>

To test the original collatz conjecture, run:

./collatz_c.o 0 1000 1000 2 1 0 2 3 1 1

The `0` and two `1000`s can  be changed depending on what you'd like to test.

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

#include "glib.h"

long long running_hash = 5813;
long long _cn = 2384912;

long long *history;

long long add_each_hash;

// function used for iteration
long long f(long long x) {
    //what is it modulo the class?
    long long res = ((x % residue_class) + residue_class) % residue_class;
    //applies operation
    return (residue_mul[res] * x + residue_add[res]) / residue_div[res];
}

// little hacky hash function to check.
void addtohash() {
    int i;
    for (i = 0; i < MAX_TRIALS; i += add_each_hash) {
        running_hash = (running_hash * (history[i] + running_hash)) % _cn;
    }
}
// main
int main(int argc, char *argv[]) {
    read_args(argv);
    
    history = (long long *)malloc(sizeof(long long) * MAX_TRIALS);
    
    print_start_info();

    add_each_hash = MAX_TRIALS / 4;

    //keep track of time
    clock_t start, end;
    // init to keep track of what repeated.
    long long x, i, r_x, trials;
    start = clock();

    // loop through their min and max, checking each
    for (x = MIN; x < MIN + RANGE; ++x) {
        trials = 0;
        r_x = x;
        current_repeated = false;
        while (trials < MAX_TRIALS) {
            history[trials] = r_x;
            r_x = f(r_x);
            for (i = 0; i <= trials; ++i) {
                if (r_x == history[i]) {
                    current_repeated = true;
                }
            }
            trials++;
            if (current_repeated) {
                goto skipahead;
            }
            // we increment how many trials we've done
        }
        //label this so we can skip out
        skipahead:;
        all_repeated = all_repeated && current_repeated;
        one_repeated = one_repeated || current_repeated;
        if (!current_repeated) {
            printf("%lld does not repeat\n", x);
        } else {
            total_repeats++;
        }
        // add however many we just did
        total_trials += trials;
        addtohash();
    }

    end = clock();
    //compute elapsed microseconds (1 millionth of a second)
    elapsed_micros = (end - start);

    // print out more info
    print_end_info(elapsed_micros, total_trials);

    printf("Hash: %lld\n", running_hash);

    return 0;
}