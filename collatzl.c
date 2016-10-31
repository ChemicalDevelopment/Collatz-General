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

long long MIN;
long long MAX;
long long MAX_TRIALS;

#define get_range (double) (MAX - MIN)

long long residue_class;

long long running_hash = 5813;
long long _cn = 2384912;

long long *residue_mul;
long long *residue_add;
long long *residue_div;

long long *history;

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
    for (i = 0; i < MAX_TRIALS; i += 20) {
        running_hash += (i + 2) * history[i];
        running_hash = running_hash * (running_hash / 2 + 3) * 5 + 1523;
        running_hash = running_hash % _cn;
    }
}

// Prints info about the time
void print_time_info(double elapsed_micros) {
    //Figures out different units
    double elapsed_ms = elapsed_micros / 1000.0; double elapsed_s = elapsed_ms / 1000.0; double elapsed_m = elapsed_s / 60.0; double elapsed_hr = elapsed_m / 60.0; double elapsed_days = elapsed_hr / 24.0;
    printf("\nTook\n");
    //If it didn't take long, print out microseconds (1 millionth of a second)
    if (elapsed_micros < 60000000.0) printf("    %g micros:\n", elapsed_micros);
    //Always print out milliseconds
    printf("    %g ms:\n", elapsed_ms);
    //If it took between half a second and 2 minutes, print out seconds
    if (elapsed_s > .5 && elapsed_s < 120.0) printf("    %g s:\n", elapsed_s);
    //If it took between 30 sec and 2 hours, print out minutes
    if (elapsed_m > .5 && elapsed_m < 120.0) printf("    %g m:\n", elapsed_m);
    //If it took between 30 min and 3 days, print out hours
    if (elapsed_hr > 0.5 && elapsed_s < 72.0) printf("    %g hr:\n", elapsed_hr);
    //If it took over half a day, print out days
    if (elapsed_days > 0.5) printf("    %g days:\n", elapsed_days);
}

// prints info about trials, and numbers tested.
void print_trial_info(double elapsed_micros, long long total_trials) {
    // print out various averages
    printf("\nRan a total of %lld trials\n", total_trials);
    printf("    Average trials per microsecond: %lf\n", total_trials / elapsed_micros);
    printf("    Average numbers checked per microsecond : %lf\n", get_range / elapsed_micros);
    printf("    Average trials per number : %lf\n", (total_trials + 0.0) / get_range);
}

// prints out logging info
void print_info(double elapsed_micros, long long total_trials) {
    // print time and trial info about the computation.
    print_time_info(elapsed_micros);
    print_trial_info(elapsed_micros, total_trials);
    printf("\n");
}

// main
int main(int argc, char *argv[]) {
    // have an index to read in from argv
    int ci = 0;
    //Read in values, incrementing ci each time
    MIN = strtol(argv[++ci], NULL, 10);
    MAX = strtol(argv[++ci], NULL, 10);
    MAX_TRIALS = strtol(argv[++ci], NULL, 10);
    residue_class = strtol(argv[++ci], NULL, 10);
    residue_mul = (long long *)malloc(sizeof(long long) * residue_class);   
    residue_add = (long long *)malloc(sizeof(long long) * residue_class);   
    residue_div = (long long *)malloc(sizeof(long long) * residue_class);
    history = (long long *)malloc(sizeof(long long) * MAX_TRIALS);
    long long i;
    //loop through, reading in (mul, add, div) for each residual class
    for (i = 0; i < residue_class; ++i) {
        residue_mul[i] = strtol(argv[++ci], NULL, 10);
        residue_add[i] = strtol(argv[++ci], NULL, 10);
        residue_div[i] = strtol(argv[++ci], NULL, 10);
    }

    // print some basic info.
    printf("Running CollatzL v0.0.1\n");
    printf("Testing from %lld to %lld, going up to %lld max trials\n", MIN, MAX, MAX_TRIALS);
    printf("\n");
    // print out what they entered.
    for (i = 0; i < residue_class; ++i) {
        printf("If x %% %lld = %lld, then f(x) = ", residue_class, i);
        if (residue_div[i] != 1 && residue_add[i] != 0) printf("(");
        if (residue_mul[i] != 1) printf("%lld", residue_mul[i]);
        printf("x");
        if (residue_add[i] != 0) printf("+%lld", residue_add[i]);
        if (residue_div[i] != 1 && residue_add[i] != 0) printf(")");
        if (residue_div[i] != 1) printf("/%lld", residue_div[i]);
        printf("\n");
    }

    // print out basic question.
    printf("\nDoes the iteration of f(f(f(f(f...f(x))))) eventualy follow a pattern?\n\n");

    //keep track of time
    clock_t start, end;
    // init to keep track of what repeated.
    bool all_repeated = true, one_repeated = false, current_repeated;
    long long x, r_x, trials, total_trials = 0, total_repeats = 0;
    start = clock();
    // loop through their min and max, checking each
    for (x = MIN; x <= MAX; ++x) {
        trials = 0;
        r_x = x;
        current_repeated = false;
        while (trials < MAX_TRIALS) {
            history[trials] = r_x;
            r_x = f(r_x);
            for (i = 0; i < trials; ++i) {
                if (history[trials] == history[i] && trials != i) {
                    current_repeated = true;
                    goto skipahead;
                }
            }
            // we increment how many trials we've done
            trials += 1;
        }
        //label this so we can skip out
        skipahead:;
        all_repeated = all_repeated && current_repeated;
        one_repeated = one_repeated || current_repeated;
        if (!current_repeated) {
            printf("%lld does not repeat\n", x);
        } else {
            total_repeats += 1;
        }
        // add however many we just did
        total_trials += trials;
        addtohash();
    }
    end = clock();
    //compute elapsed microseconds (1 millionth of a second)
    double elapsed_micros = (end - start);

    // print out special message if they all repeated
    if (all_repeated) {
        printf("All tested inputs repeat (%lld total)\n", total_repeats);
    } else {
        printf("%lld numbers repeated\n", total_repeats);
    }

    // print out more info
    print_info(elapsed_micros, total_trials);


    printf("Hash: %lld\n", running_hash);

    // free memory
    free(residue_add);
    free(residue_mul);
    free(residue_div);
    free(history);
    return 0;
}