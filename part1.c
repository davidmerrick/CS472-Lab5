//
// Adapted from code found at http://www.cplusplus.com/forum/general/35557/
// Determine cache size programmatically by writing to an array repeatedly
// Cache size for Atom D510 processor I have:
//		L1: ~24Kb
//		L2: 1x or 2x 512K (I think it's the latter)
//
// Cache line size: 64-byte
//
// So array should be about
//		(1.25 * (Sum of Cache Sizes))/(Array element size)
//		(1.25 * (24K + 1M))/8
//		= 160,000 Bytes
//
// Notes:
//		sizeof(int): 4 bytes
//		sizeof(long long int): 8 bytes
//		sizeof(char): 1 byte

#define KB 1024
#define MB 1024 * 1024

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//Given an array of times, return the average
clock_t get_average_time(clock_t *time_array, int array_size){
	clock_t sum = 0;
	for(int i = 0; i < array_size; i++){
		sum += time_array[i];
	}
	return sum/array_size;
}


void get_cache_size_two(){
		//Open file to store results
		FILE* fh = fopen("cachelines.csv", "w");

		//Headers
		fprintf(fh, "Step Size, Time Taken \n");

		if (!fh){
			//error
			return 1;
		}

		int steps = 64 * 1024 * 1024;
    int arr[1024 * 1024];
    int lengthMod = (1024 * 1024) - 1; //Makes sure everything stays within the array
    int i;
    double timeTaken;
    clock_t start;

		for(int step_size = 1; step_size < 1024; step_size *= 2){
	    start = clock();
	    for (i = 0; i < steps; i+=step_size) {
	        arr[(i * 16) & lengthMod]++;
	    }
			timeTaken = (double)(clock() - start)/CLOCKS_PER_SEC;
			fprintf(fh, "%d, %.12f \n", step_size, timeTaken);
		}

		fclose(fh);
}

clock_t thrash_cache(int cache_line_increment){
	//160,000 / sizeof(long long int) = 20,000
	int buffer_size = 2e4;

	long long int buf[buffer_size];

	//Measure the time of the operation. Steep increase in time indicates cache border
	clock_t start = clock();

	for(int i = 0; i < buffer_size; i += cache_line_increment){
		//Write random data to the buffer
		long long int rando = (long long int) rand();
		buf[i] = rando;
	}

	clock_t elapsed = clock() - start;

	return elapsed;
}

//Attempts to find out the size of a cache line
//Based on http://igoro.com/archive/gallery-of-processor-cache-effects/
int get_cache_line_size(){
	//Open file to store results
	FILE* fh = fopen("cachelines.csv", "w");

	if (!fh){
		//error
		return 1;
	}

	int max_cache_line_size, results_array_size;
	//Set it big so results average out
	max_cache_line_size = results_array_size = 30000;

	//1. Establish a baseline
	int buffer_size = 3 * max_cache_line_size;

	char *buf = malloc(sizeof(char) * buffer_size);

	clock_t start = clock();

	for(int i = 0; i < buffer_size; i++){
		//Write random data to the buffer
		char rando = (char) rand() % 128;
		memcpy(buf + i, &rando, sizeof(char));
	}

	clock_t baseline = clock() - start;

	fprintf(fh, "Baseline: %lu\n", baseline);

	//2. Use the baseline to compare performance to
	//(Record results for debugging. And write them out.)
	fprintf(fh, "Headers: step size, time\n");
	clock_t results[max_cache_line_size];
	int j = 0; //Index in results array

	for(int step_size = 1; step_size < max_cache_line_size; step_size *= 2){
		char *buf2 = realloc(buf, sizeof(char) * buffer_size);

		clock_t start = clock();

		for(int i = 0; i < buffer_size; i += step_size){
			//Write random data to the buffer
			char rando = (char) rand() % 128;
			memcpy(buf2 + i, &rando, sizeof(char));
		}

		clock_t elapsed = clock() - start;
		//		if(elapsed >= (0.8) * baseline){
		//			return j;
		//		}
		fprintf(fh, "%d, %lu\n", step_size, elapsed);

		results[j] = elapsed;
		j++;
	}

	free(buf);

	clock_t average = get_average_time(results, results_array_size);

	fclose(fh);
}

int main(){
	printf("Getting cache line size\n");
	get_cache_size_two();

	//	printf("Writing array access timings to \"results.csv\"\n");
	//
	//	FILE* fh = fopen("results.csv", "w");
	//
	//	if (!fh){
	//		//error
	//		return 1;
	//	}
	//
	//	int cache_line_increment;
	//	for (cache_line_increment = 8; cache_line_increment < 16 * 1024 * 1024; cache_line_increment *= 2){
	//		fprintf(fh, "%d, %lu\n", cache_line_increment, thrash_cache(cache_line_increment));
	//		printf(".");
	//		fflush(stdout);
	//	}
	//
	//	fclose(fh);

	return 0;
}
