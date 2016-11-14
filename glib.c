/*

This is a library for collatzl

Keeps track of the results, and reports info in the same format for each version

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "glib.h"

// Min and and range to test, up to max_trials. Residue class is the modulo used to determine function
long long MIN, RANGE, MAX_TRIALS, residue_class;

// pointers to the arrays of residue arithmetics
long long *residue_mul, *residue_add, *residue_div;

// total number of trials used, and the amount of repeats we have seen
long long total_trials = 0, total_repeats = 0;

// have they all repeated, has even one repeated, and has the current repeated
bool all_repeated = true, one_repeated = false, current_repeated;

// how many microseconds? 1000000 microseconds = 1 second
double elapsed_micros;

// prints beginning info
void print_start_info() {
	printf("Running CollatzL v0.1.0\n");
    printf("Testing from %lld to %lld, going up to %lld max trials\n\n", MIN, MIN + RANGE, MAX_TRIALS);
    long long i;
    // print out what they entered as an equation
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
    // this is the question we try to answer
	printf("\nDoes the iteration of f(f(f(f(f...f(x))))) eventualy follow a pattern?\n\n");
}

// Prints info about the time
void print_time_info() {
    //Figures out different units
    double elapsed_ms = elapsed_micros / 1000.0; double elapsed_s = elapsed_ms / 1000.0; double elapsed_m = elapsed_s / 60.0; double elapsed_hr = elapsed_m / 60.0; double elapsed_days = elapsed_hr / 24.0;
    printf("\nTook\n");
    //If it didn't take long, print out microseconds (1 millionth of a second)
    if (elapsed_micros < 60000000.0) printf("    %g micros\n", elapsed_micros);
    //Always print out milliseconds
    printf("    %g ms:\n", elapsed_ms);
    //If it took between half a second and 2 minutes, print out seconds
    if (elapsed_s > .5 && elapsed_s < 120.0) printf("    %g s\n", elapsed_s);
    //If it took between 30 sec and 2 hours, print out minutes
    if (elapsed_m > .5 && elapsed_m < 120.0) printf("    %g m\n", elapsed_m);
    //If it took between 30 min and 3 days, print out hours
    if (elapsed_hr > 0.5 && elapsed_hr < 72.0) printf("    %g hr\n", elapsed_hr);
    //If it took over half a day, print out days
    if (elapsed_days > 0.5) printf("    %g days\n", elapsed_days);
}

// prints info about trials, and numbers tested.
void print_trial_info() {
    // print out various averages
    printf("\nRan a total of %lld trials\n", total_trials);
    printf("    Average trials per microsecond: %lf\n", total_trials / elapsed_micros);
    printf("    Average numbers checked per microsecond : %lf\n", RANGE / elapsed_micros);
    printf("    Average trials per number : %lf\n", (total_trials + 0.0) / RANGE);
}

// prints out logging info
void print_end_info() {
		    // print out special message if they all repeated
    if (all_repeated) {
        fprintf(stderr, "All tested inputs repeat (%lld total)\n", total_repeats);
    } else if (one_repeated) {
        fprintf(stderr, "%lld numbers repeated\n", total_repeats);
    } else {
        fprintf(stderr, "None repeated\n");
    }
    if (!all_repeated) {
        fprintf(stderr, "Not all repeated\n");
    }
    // print time and trial info about the computation.
    print_time_info(elapsed_micros);
    print_trial_info();
    printf("\n");
}

// reads arguments from the arguments
void read_args(char *argv[]) {
    // have an index to read in from argv
    int ci = 0;
    //Read in values, incrementing ci each time
    MIN = strtoll(argv[++ci], NULL, 10);
    RANGE = strtoll(argv[++ci], NULL, 10);
    MAX_TRIALS = strtoll(argv[++ci], NULL, 10);
    residue_class = strtoll(argv[++ci], NULL, 10);
    residue_mul = (long long *)malloc(sizeof(long long) * residue_class);   
    residue_add = (long long *)malloc(sizeof(long long) * residue_class);   
    residue_div = (long long *)malloc(sizeof(long long) * residue_class);
    long long i;
    //loop through, reading in (mul, add, div) for each residual class
    for (i = 0; i < residue_class; ++i) {
        residue_mul[i] = strtoll(argv[++ci], NULL, 10);
        residue_add[i] = strtoll(argv[++ci], NULL, 10);
        residue_div[i] = strtoll(argv[++ci], NULL, 10);
    }
}
