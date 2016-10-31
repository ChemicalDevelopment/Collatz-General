/*

Run like

./run_compute.o <residue class> <mul . . . > <add . . . > <div . . .> 


*/

#include <stdio.h>
#include <stdlib.h>

int MAX = 100;
int MAX_TRIALS = 100;

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
    residue_class = strtol(argv[++ci], NULL, 10);
    residue_mul = (int *)malloc(sizeof(int) * residue_class);   
    residue_add = (int *)malloc(sizeof(int) * residue_class);   
    residue_div = (int *)malloc(sizeof(int) * residue_class);
    history = (int *)malloc(sizeof(int) * MAX_TRIALS);
    int i;
    for (i = 0; i < residue_class; ++i) residue_mul[i] = strtol(argv[++ci], NULL, 10);
    for (i = 0; i < residue_class; ++i) residue_add[i] = strtol(argv[++ci], NULL, 10);
    for (i = 0; i < residue_class; ++i) residue_div[i] = strtol(argv[++ci], NULL, 10);

    int x, r_x, trials, repeats;
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
            printf("%d repeats\n", x);
        }
    }
    free(residue_add);
    free(residue_mul);
    free(residue_div);
    free(history);
}
