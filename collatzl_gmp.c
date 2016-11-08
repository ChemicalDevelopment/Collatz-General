#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <gmp.h>

#include "glib.h"

mpz_t *history;

mpz_t tmp;

unsigned int res, rm, ra, rd;


// function used for iteration
void f(mpz_t x) {
	res = mpz_fdiv_r_ui(tmp, x, residue_class);
	rm = residue_mul[res];
	ra = residue_add[res];
	rd = residue_div[res];
    mpz_mul_ui(x, x, rm);
	mpz_add_ui(x, x, ra);
	mpz_div_ui(x, x, rd);
}

// main
int main(int argc, char *argv[]) {
    read_args(argv);
	//keep track of time
    clock_t start, end;
    // init to keep track of what repeated.
    long long x, i, trials;
	mpz_t r_x;
	mpz_init(r_x);
	mpz_init(tmp);
    history = (mpz_t *)malloc(sizeof(mpz_t) * MAX_TRIALS);
	for (i = 0; i < MAX_TRIALS; ++i) {
		mpz_init(history[i]);
	}
    
    print_start_info();
    start = clock();

    // loop through their min and max, checking each
    for (x = MIN; x < MIN + RANGE; ++x) {
        trials = 0;
        mpz_set_ui(r_x, x);
        current_repeated = false;
        while (trials < MAX_TRIALS) {
			mpz_set(history[trials], r_x);
            f(r_x);
            trials++;
            for (i = 0; i < trials; ++i) {
                if (mpz_cmp(r_x, history[i]) == 0) {
                    goto skipahead;
                }
            }
            // we increment how many trials we've done
        }
        //label this so we can skip out
        skipahead:current_repeated = true;
        all_repeated = all_repeated && current_repeated;
        one_repeated = one_repeated || current_repeated;
        if (!current_repeated) {
            printf("%lld does not repeat\n", x);
        } else {
            total_repeats++;
        }
        // add however many we just did
        total_trials += trials;
    }

    end = clock();
    //compute elapsed microseconds (1 millionth of a second)
    elapsed_micros = (end - start);

    // print out more info
    print_end_info(elapsed_micros, total_trials);

    return 0;
}
