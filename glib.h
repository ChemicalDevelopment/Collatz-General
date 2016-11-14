
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

long long MIN;
long long RANGE;
long long MAX_TRIALS;

long long residue_class;

long long *residue_mul, *residue_add, *residue_div;

long long total_trials, total_repeats;

bool all_repeated, one_repeated, current_repeated;

double elapsed_micros;

void print_start_info();

// Prints info about the time
void print_time_info();

// prints info about trials, and numbers tested.
void print_trial_info();

// prints out logging info
void print_end_info();

void read_args(char *argv[]);