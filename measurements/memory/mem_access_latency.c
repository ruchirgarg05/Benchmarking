#include "../../utils/utils.h"
#include <math.h>

#undef NUM_ITERATIONS
#define NUM_ITERATIONS 100
/**
 * Similar approach as LMBench, although not as efficent:
 * 
 * 1. We allocate an array of integers of total size 2^n bytes.
 * 2. For each memory size, we access all elements of the array.
 *    - For arrays < size(L1) and arrays < size(L2), elements will
 *      be fetched from cache lines on iterations after the first.
*/
int main() {


    // 2^n[i] defines total amount of memory, 2^24 = 16MB
    int n[] = { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
	       18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
               28, 29, 30, 31, 32, 33, 34 
    }; 

    int bytes = (int)(pow(2, 24));
    char* start = malloc(bytes);

    for (int i=0; i<16; i++) {
        // Run Iterations
	if (i != 2)
	{
		continue;
	}
	
	int n_elems = (int)(pow(2, n[i]));

        float cycles_per_load = 0;
        for (int j=0; j<NUM_ITERATIONS; j++) {
    	    struct Timer timer;

            tic(timer);
            for (int k=0; k<n_elems; k++) {
                char* elem = start + k;
            }
            toc(timer);

            cycles_per_load += (float)timer_diff(timer) / (float)n_elems; // TODO: Check for overflow
        }

        printf("%d,%f\n", n_elems, cycles_per_load / NUM_ITERATIONS);
    }

    free(start);
    return 0;
}
