/*
 * Author: Joshua Kluthe
 * Date: 2017.05.01
 * 
 * Description: This program compares performance between static and dynamic 
 * scheduling and between using large and small chunk sizes.
 * 
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <omp.h>

#define ARRYSIZE 8192

float arry[ARRYSIZE];

float rand_float(unsigned int *seedp, float low, float high);

//NUMT = threads used
//SCHED = dynamic or static scheduling
//CHUNK = chunk size of 1 or 4096
//argv[1] = name of file to append data to
int main(int argc, char *argv[]) {
	
#ifndef _OPENMP
    std::fprintf(stderr, "OpenMP not supported, exiting program.\n");
    return 1;
#endif

	//open data file	
	FILE *datafile = std::fopen(argv[1], "a");
	
	//fill array
	unsigned int seed = 0;
	for(int i = 0; i < ARRYSIZE; i++)
		arry[i] = rand_float(&seed, -1.0, 1.0);

	//set thread count
	omp_set_num_threads(NUMT);
	
	//calculate number of multiplications to be performed
	long int nums_muled = (long int) ARRYSIZE * (long int) (ARRYSIZE + 1) / 2;

	double mega_mults = 0;

	//outer loop gets 100 data points that we will average
	for(int k = 0; k < 100; k++) {

		double t_not = omp_get_wtime();

		#pragma omp parallel for default(none), shared(arry), schedule(SCHED, CHUNK)
		for(int i = 0; i < ARRYSIZE; i++) {
	
			float product = 1;
		
			for(int j = 0; j < i; j++) {
				product *= arry[j];			
			}		
		}
		mega_mults += (double) nums_muled / (omp_get_wtime() - t_not) / 1000000;
	}

	std::fprintf(datafile, "%lf, ", mega_mults/100);
	std::fclose(datafile);
		
	return 0;	
}



float rand_float(unsigned int *seedp, float low, float high) {

	float r = (float) rand_r(seedp);

	return(low + r * (high - low) / (float) RAND_MAX);
}


