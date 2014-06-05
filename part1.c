//
// Determine cache size programmatically by writing to an array repeatedly
// Cache size for Atom D510 processor I have:
//		L1: ~24Kb
//		L2: 1x or 2x 512K (I think it's the latter)
//
// Cache line size: 64-byte
//
//
// This works just by looking at the results.csv and graphing it out.
// Spikes in time taken indicate the cache line and cache size.
// Could be done automatically using calculus to figure out average slope.
// A slope greater than this between 2 data points could indicate a step.
// First step = cache line. 2nd step = L1 cache.

#define KB 1024
#define MB 1024 * 1024

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void get_cache_size(){
		//Based on: https://gist.github.com/jiewmeng/3787223#file_cache_profile.c
		//Open file to store results
		FILE* fh = fopen("results.csv", "w");

		//Headers
		fprintf(fh, "Array size (KB), Time Taken \n");

		if (!fh){
			//error
			return;
		}

		unsigned int steps = 256 * 1024 * 1024;
    static int arr[4 * 1024 * 1024];
		int lengthMod; //Makes sure everything stays within the array
    int i;
    double timeTaken;
    clock_t start;
		int sizes[] = {
        1 * KB, 4 * KB, 8 * KB, 16 * KB, 32 * KB, 64 * KB, 128 * KB, 256 * KB,
        512 * KB, 1 * MB, 1.5 * MB, 2 * MB, 2.5 * MB, 3 * MB, 3.5 * MB, 4 * MB
    };
    int results[sizeof(sizes)/sizeof(int)];
    int s;

    // Test each size
    for (s = 0; s < sizeof(sizes)/sizeof(int); s++) {
	    lengthMod = sizes[s] - 1;
	    start = clock();
	    for (i = 0; i < steps; i++) {
	        arr[(i * 16) & lengthMod] *= 10;
            arr[(i * 16) & lengthMod] /= 10;
	    }

	    timeTaken = (double)(clock() - start)/CLOCKS_PER_SEC;
			fprintf(fh, "%d, %.12f \n", sizes[s] / KB, timeTaken);
    }

		fclose(fh);
}

int main(){
	printf("Writing results to \"results.csv\"\n");
	get_cache_size();
	return 0;
}
