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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>


//Given an array of times, return the average
clock_t get_average_time(clock_t *time_array, int array_size){
	clock_t sum = 0;
	for(int i = 0; i < array_size; i++){
		sum += time_array[i];
	}
	return sum/array_size;
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
	int max_cache_line_size, results_array_size;
	//Set it big so results average out
	max_cache_line_size = results_array_size = 4096;
	
	//1. Establish a baseline
	int buffer_size = 3 * max_cache_line_size;
	
	char buf[buffer_size];
	
	clock_t start = clock();
	
	for(int i = 0; i < buffer_size; i++){
		//Write random data to the buffer
		char rando = (char) rand() % 128;
		buf[i] = rando;
	}
	
	clock_t baseline = clock() - start;
	
	//2. Use the baseline to compare performance to
	//(Record results for debugging. And write them out.)
	clock_t results[max_cache_line_size];
	int j = 0; //Index in results array
	
	FILE* fh = fopen("cachelines.csv", "w");
	
	if (!fh){
		//error
		return 1;
	}
	
	for(int step_size = 1; step_size < max_cache_line_size; step_size *= 2){
		buffer_size = 3 * max_cache_line_size;
		
		clock_t start = clock();
		
		for(int i = 0; i < buffer_size; i += step_size){
			//Write random data to the buffer
			char rando = (char) rand() % 128;
			buf[i] = rando;
		}
		
		clock_t elapsed = clock() - start;
//		if(elapsed >= (0.8) * baseline){
//			return j;
//		}
		
		results[j] = elapsed;
		j++;
	}
	
	clock_t average = get_average_time(results, results_array_size);
	
	fclose(fh);
	
	return 0;
}

int main(){
	printf("Getting cache line size\n");
	get_cache_line_size();

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
