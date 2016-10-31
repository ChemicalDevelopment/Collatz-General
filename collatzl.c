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

long MIN;
long MAX;
long MAX_TRIALS;

#define get_range MAX - MIN

long residue_class;

long *residue_mul;
long *residue_add;
long *residue_div;

long *history;

long transform_num(long x) {
    long res = ((x % residue_class) + residue_class) % residue_class;
    return (residue_mul[res] * x + residue_add[res]) / residue_div[res];
}

void print_time_info(double elapsed_micros) {
    double elapsed_ms = elapsed_micros / 1000.0;
    double elapsed_s = elapsed_ms / 1000.0;
    double elapsed_hr = elapsed_s / 3600.0;
    double elapsed_days = elapsed_hr / 24.0;
    printf("\nTook\n");
    if (elapsed_micros < 60000000.0) printf("    %g micros:\n", elapsed_micros);
    printf("    %g ms:\n", elapsed_ms);
    if (elapsed_s > .5 && elapsed_s < 30.0) printf("    %g s:\n", elapsed_s);
    if (elapsed_hr > 0.5 && elapsed_s < 30.0) printf("    %g hr:\n", elapsed_hr);
    if (elapsed_days > 0.5) printf("    %g days:\n", elapsed_days);
}

void print_trial_info(double elapsed_micros, long total_trials) {
    printf("\nRan a total of %ld trials\n", total_trials);
    printf("    Average trials per microsecond: %lf\n", total_trials / elapsed_micros);
    printf("    Average numbers checked per microsecond : %lf\n", get_range / elapsed_micros);
    printf("    Average trials per number : %lf\n", (total_trials + 0.0) / get_range);
}

void print_info(double elapsed_micros, long total_trials) {
    print_time_info(elapsed_micros);
    print_trial_info(elapsed_micros, total_trials);
    printf("\n");
}

int main(int argc, char *argv[]) {
    int ci = 0;
    MIN = strtol(argv[++ci], NULL, 10);
    MAX = strtol(argv[++ci], NULL, 10);
    MAX_TRIALS = strtol(argv[++ci], NULL, 10);
    residue_class = strtol(argv[++ci], NULL, 10);
    residue_mul = (long *)malloc(sizeof(long) * residue_class);   
    residue_add = (long *)malloc(sizeof(long) * residue_class);   
    residue_div = (long *)malloc(sizeof(long) * residue_class);
    history = (long *)malloc(sizeof(long) * MAX_TRIALS);
    long i;
    for (i = 0; i < residue_class; ++i) {
        residue_mul[i] = strtol(argv[++ci], NULL, 10);
        residue_add[i] = strtol(argv[++ci], NULL, 10);
        residue_div[i] = strtol(argv[++ci], NULL, 10);
    }

    printf("Running CollatzL v0.0.1\n");
    printf("Testing from %ld to %ld, going up to %ld max trials\n", MIN, MAX, MAX_TRIALS);
    printf("\n");
    for (i = 0; i < residue_class; ++i) {
        printf("If x %% %ld = %ld, then f(x) = ", residue_class, i);
        if (residue_div[i] != 1 && residue_add[i] != 0) printf("(");
        if (residue_mul[i] != 1) printf("%ld", residue_mul[i]);
        printf("x");
        if (residue_add[i] != 0) printf("+%ld", residue_add[i]);
        if (residue_div[i] != 1 && residue_add[i] != 0) printf(")");
        if (residue_div[i] != 1) printf("/%ld", residue_div[i]);
        printf("\n");
    }   
    printf("\nDoes the iteration of f(f(f(f(f...f(x))))) eventualy follow a pattern?\n\n");


    clock_t start, end;
    bool all_repeated = true, one_repeated = false, current_repeated;
    long x, r_x, trials, total_trials = 0, total_repeats = 0;
    start = clock();
    for (x = MIN; x <= MAX; ++x) {
        trials = 0;
        r_x = x;
        current_repeated = false;
        while (trials < MAX_TRIALS) {
            history[trials] = r_x;
            r_x = transform_num(r_x);
            for (i = 0; i < trials; ++i) {
                if (history[trials] == history[i] && trials != i) {
                    current_repeated = true;
                    goto skipahead;
                }
            }
            trials += 1;
            total_trials += 1;
        }
        skipahead:;
        all_repeated = all_repeated && current_repeated;
        one_repeated = one_repeated || current_repeated;
        if (!current_repeated) {
            printf("%ld does not repeat\n", x);
        } else {
            total_repeats += 1;
        }
    }
    end = clock();
    double elapsed_micros = (end - start);

    if (all_repeated) {
        printf("All tested inputs repeat (%ld total)\n", total_repeats);
    } else {
        printf("%ld numbers repeated\n", total_repeats);
    }

    print_info(elapsed_micros, total_trials);

    free(residue_add);
    free(residue_mul);
    free(residue_div);
    free(history);
}