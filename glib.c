#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "glib.h"

long long MIN;
long long RANGE;
long long MAX_TRIALS;

long long residue_class;


long long *residue_mul;
long long *residue_add;
long long *residue_div;

long long total_trials = 0, total_repeats = 0;

bool all_repeated = true, one_repeated = false, current_repeated;

double elapsed_micros;

void print_start_info() {
	printf("Running CollatzL v0.0.1\n");
    printf("Testing from %lld to %lld, going up to %lld max trials\n", MIN, MIN + RANGE, MAX_TRIALS);
    printf("\n");
    long long i;
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

	printf("\nDoes the iteration of f(f(f(f(f...f(x))))) eventualy follow a pattern?\n\n");
}

// Prints info about the time
void print_time_info() {
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
        printf("All tested inputs repeat (%lld total)\n", total_repeats);
    } else {
        printf("%lld numbers repeated\n", total_repeats);
    }
    // print time and trial info about the computation.
    print_time_info(elapsed_micros);
    print_trial_info();
    printf("\n");
}

void read_args(char *argv[]) {
// have an index to read in from argv
    int ci = 0;
    //Read in values, incrementing ci each time
    MIN = strtol(argv[++ci], NULL, 10);
    RANGE = strtol(argv[++ci], NULL, 10);
    MAX_TRIALS = strtol(argv[++ci], NULL, 10);
    residue_class = strtol(argv[++ci], NULL, 10);
    residue_mul = (long long *)malloc(sizeof(long long) * residue_class);   
    residue_add = (long long *)malloc(sizeof(long long) * residue_class);   
    residue_div = (long long *)malloc(sizeof(long long) * residue_class);
    long long i;
    //loop through, reading in (mul, add, div) for each residual class
    for (i = 0; i < residue_class; ++i) {
        residue_mul[i] = strtol(argv[++ci], NULL, 10);
        residue_add[i] = strtol(argv[++ci], NULL, 10);
        residue_div[i] = strtol(argv[++ci], NULL, 10);
    }
}
