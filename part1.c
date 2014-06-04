//Adapted from code found at http://www.cplusplus.com/forum/general/35557/
//Determine cache size programmatically by writing to an array repeatedly


#include <time.h>
#include <stdio.h>
#include <stdlib.h>


clock_t whack_cache(long long int sz){
	char* buf[sz];

	//Measure the time of the operation. Steep increase in time indicates cache border
	clock_t start = clock();

	long long int i;
	for(i = 0; i < 64 * 1024 * 1024; i++){
        	++buf[(i * 64) % sz];
	}

	clock_t elapsed = clock() - start;

	return elapsed;
}

int main(){
	printf("Writing array access timings to \"results.csv\"");;

	FILE* fh = fopen("results.csv", "w");
	
	if (!fh){
		//error
        	return 1;
	}
   
	long long int sz; 
	for (sz = 1024; sz < 16 * 1024 * 1024; sz *= 2){
		fprintf(fh, "%d, %lu\n", sz/1024, whack_cache(sz));
		printf(".");
		fflush(stdout);
	}

	fclose(fh);

	return 0;
}
