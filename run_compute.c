/*

Run like

./run_compute.o <max> <max_trials> <residue class> <mul . . . > <add . . . > <div . . .> 


*/

#include <stdio.h>
#include <stdlib.h>

int MAX;
int MAX_TRIALS;

int residue_class;

int *residue_mul;
int *residue_add;
int *residue_div;

int *history;

int transform_num(int x) {
    int res = x % residue_class;
    return (residue_mul[res] * x + residue_add[res]) / residue_div[res];
}

int main(int argc, char *argv[]) {
    int ci = 0;
    MAX = strtol(argv[++ci], NULL, 10);
    MAX_TRIALS = strtol(argv[++ci], NULL, 10);
    residue_class = strtol(argv[++ci], NULL, 10);
    residue_mul = (int *)malloc(sizeof(int) * residue_class);   
    residue_add = (int *)malloc(sizeof(int) * residue_class);   
    residue_div = (int *)malloc(sizeof(int) * residue_class);
    history = (int *)malloc(sizeof(int) * MAX_TRIALS);
    int i;
    for (i = 0; i < residue_class; ++i) residue_mul[i] = strtol(argv[++ci], NULL, 10);
    for (i = 0; i < residue_class; ++i) residue_add[i] = strtol(argv[++ci], NULL, 10);
    for (i = 0; i < residue_class; ++i) residue_div[i] = strtol(argv[++ci], NULL, 10);

    printf("Running CollatzL v0.0.1\n");
    printf("Testing from 0 to %d, going up to %d max trials\n", MAX, MAX_TRIALS);
    printf("\n");
    for (i = 0; i < residue_class; ++i) {
        printf("If x %% %d = %d, then f(x) = ", residue_class, i);
        if (residue_div[i] != 1 && residue_add[i] != 0) printf("(");
        if (residue_mul[i] != 1) printf("%d", residue_mul[i]);
        printf("x");
        if (residue_add[i] != 0) printf("+%d", residue_add[i]);
        if (residue_div[i] != 1 && residue_add[i] != 0) printf(")");
        if (residue_div[i] != 1) printf("/%d", residue_div[i]);
        printf("\n");
    }   

    printf("Does the iteration of f(f(f(f(f...f(x))))) eventualy follow a pattern?\n");

    int x, r_x, trials, repeats, all_repeated = 1;
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
        if (repeats == 1) {
            //printf("%d repeats\n", x);
        } else {
            printf("%d does not repeat\n", x);
        }
        all_repeated = all_repeated & repeats;
    }

    if (all_repeated == 1) {
        printf("All tested inputs repeat\n");
    }

    free(residue_add);
    free(residue_mul);
    free(residue_div);
    free(history);
}
