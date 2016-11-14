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

Without special cases:
426
423


*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// our library
#include "glib.h"

long long running_hash = 5813;
long long _cn = 2384912;

long long x, i, r_x, trials;


long long *history;

long long add_each_hash, res;

long long overflow = 0;

// function used for iteration
long long f(long long x) {
    //what is it modulo the class?
    res = ((x % residue_class) + residue_class) % residue_class;
    //applies operation
    //printf("%lld\n", LLONG_MAX / (x == 0 ? 1 : x));
    if (x >= LLONG_MAX / residue_mul[res] - 1) {
        printf("Overflow detected!\n");
        overflow = 1;
    }
    return (residue_mul[res] * x + residue_add[res]) / residue_div[res];
}

int check_hist() {
    for (i = 0; i < trials; ++i) {
        if (r_x == history[i]) {
            return true;
        }
    }
    return false;
}

long long f_opt(long long x) {
    res = x & 1;

    if (res) {
        if (x >= LLONG_MAX / 3 - 1) {
            printf("Overflow detected!\n");
            overflow = 1;
        }
        return 3 * x + 1;
    }
    return x >> 1;
 }

int check_hist_opt() {
    return (r_x == 1) || (r_x == 2) || (r_x == 4);
}

// little hacky hash function to check.
void addtohash() {
    int i;
    for (i = 0; i < MAX_TRIALS; i += add_each_hash) {
        running_hash = (running_hash * (history[i] + running_hash)) % _cn;
    }
}

int eq_coef(long long *a, long long *b) {
    return (a[0]==b[0]) && (a[1]==b[1]);
}

// main
int main(int argc, char *argv[]) {

    // function pointer for unoptimized version
    long long (*ptr)(long long) = &f;
    int (*hist_ptr)() = &check_hist;


    read_args(argv);

    long long opt_mul[2] = {1, 3}, opt_add[2] = {0, 1}, opt_div[2] = {2, 1};

    if (residue_class == 2 && eq_coef(opt_mul, residue_mul) && eq_coef(opt_add, residue_add) && eq_coef(opt_div, residue_div)) {
        printf("Using optimized function for the original collatz conjecture\n");
        ptr = &f_opt;
        hist_ptr = &check_hist_opt;
        
    }

    history = (long long *)malloc(sizeof(long long) * MAX_TRIALS);
    
    add_each_hash = MAX_TRIALS / 4;

    print_start_info();

    //keep track of time
    clock_t start, end;
    // init to keep track of what repeated.
    start = clock();

    // loop through their min and max, checking each
    for (x = MIN; x < MIN + RANGE; ++x) {
        trials = 0;
        r_x = x;
        current_repeated = false;
        while (trials < MAX_TRIALS) {
            history[trials] = r_x;
            // r_x = f(r_x);
            r_x = (*ptr)(r_x);
            current_repeated = (*hist_ptr)();

            trials++;
            if (current_repeated) goto skipahead;
            // we increment how many trials we've done
        }
        //label this so we can skip out
        skipahead:;
        all_repeated = all_repeated && current_repeated;
        one_repeated = one_repeated || current_repeated;
        if (!current_repeated) {
            fprintf(stderr, "%lld does not repeat\n", x);
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

    if (overflow) {
        printf("Overflow detected. Results not accurate\n");
    }

    return 0;
}