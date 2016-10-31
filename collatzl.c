/*

Run like

./run_compute.o <max> <max_trials> <residue class> <mul . . . > <add . . . > <div . . .> 


*/

#include <stdio.h>
#include <stdlib.h>

long MAX;
long MAX_TRIALS;

long residue_class;

long *residue_mul;
long *residue_add;
long *residue_div;

long *history;

long transform_num(long x) {
    long res = ((x % residue_class) + residue_class) % residue_class;
    return (residue_mul[res] * x + residue_add[res]) / residue_div[res];
}

int main(int argc, char *argv[]) {
    int ci = 0;
    MAX = strtol(argv[++ci], NULL, 10);
    MAX_TRIALS = strtol(argv[++ci], NULL, 10);
    residue_class = strtol(argv[++ci], NULL, 10);
    residue_mul = (long *)malloc(sizeof(long) * residue_class);   
    residue_add = (long *)malloc(sizeof(long) * residue_class);   
    residue_div = (long *)malloc(sizeof(long) * residue_class);
    history = (long *)malloc(sizeof(long) * MAX_TRIALS);
    long i;
    for (i = 0; i < residue_class; ++i) residue_mul[i] = strtol(argv[++ci], NULL, 10);
    for (i = 0; i < residue_class; ++i) residue_add[i] = strtol(argv[++ci], NULL, 10);
    for (i = 0; i < residue_class; ++i) residue_div[i] = strtol(argv[++ci], NULL, 10);

    printf("Running CollatzL v0.0.1\n");
    printf("Testing from 0 to %ld, going up to %ld max trials\n", MAX, MAX_TRIALS);
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


    long x, r_x, trials, repeats, all_repeated = 1, one_repeated = 0;
    for (x = 0; x < MAX; ++x) {
        trials = 0;
        r_x = x;
        repeats = 0;
        while (trials < MAX_TRIALS) {
            history[trials] = r_x;
            r_x = transform_num(r_x);
            for (i = 0; i < trials; ++i) {
                if (history[trials] == history[i]) {
                    repeats = 1;
                }
            }
            trials += 1;
        } 
        all_repeated = all_repeated & repeats;
        one_repeated = one_repeated | repeats;
        if (!repeats) {
            printf("%ld does not repeat\n", x);
            break;
        }
    }

    if (all_repeated) {
        printf("All tested inputs repeat\n");
    }

    if (one_repeated & !all_repeated) {
        printf("At least one repeated\n");
    }

    free(residue_add);
    free(residue_mul);
    free(residue_div);
    free(history);
}
