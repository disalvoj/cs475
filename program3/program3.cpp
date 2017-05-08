/*
 * Author: Joshua Kluthe
 * Date: 2017.05.01
 * 
 * Description: This program compares using padding vs using a private temp 
 * variable to fix false sharing issues.
 * 
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <omp.h>

#define ARRAYSIZE 4

struct s {
	float value;
	int pad[NUMPAD];
} arry[ARRAYSIZE];

//number of inner loop iterations
const int iters = 10000000;

float rand_float(unsigned int *seedp, float low, float high);

//FIX 1: we use padding, FIX 2: we use temp variable
//NUMT = number of threads
//NUMPAD = number of ints to pad the struct with
//argv[1] = name of file to append data to
int main(int argc, char *argv[]) {
	
#ifndef _OPENMP
    std::fprintf(stderr, "OpenMP not supported, exiting program.\n");
    return 1;
#endif

	//open data file	
	FILE *datafile = std::fopen(argv[1], "a");

	//set thread count
	omp_set_num_threads(NUMT);

	//variables for finding average and peak performance
	int ave_iters = 100;
	double average = 0;
	double peak = 0;

	//outer outer loop for finding average and peak performance	
	for(int k = 0; k < ave_iters; k++) {	

		double t_not = omp_get_wtime();

		if(FIX == 1) {
			//FIX 1
			#pragma omp parallel for default(none), shared(arry)
			for(int i = 0; i < ARRAYSIZE; i++) {

				//private seed for rand_float()
				unsigned int seed = 0;

				for(unsigned int j = 0; j < iters; j++) {

					arry[i].value += rand_float(&seed, -1., 1.);
				}
		
			}

		} else {
			//FIX 2
			#pragma omp parallel for default(none), shared(arry)
			for(int i = 0; i < ARRAYSIZE; i++) {

				//private seed for rand_float()
				unsigned int seed = 0;

				//create temp private variable for FIX 2
				float temp = arry[i].value;
			
				for(unsigned int j = 0; j < iters; j++) {

					temp += rand_float(&seed, -1., 1.);

				}
				arry[i].value = temp;
			}
		}
		
		if((double)ARRAYSIZE * (double)iters / (omp_get_wtime() - t_not) > peak)
			peak = (double)ARRAYSIZE * (double)iters / (omp_get_wtime() - t_not);
	
		average += (double)ARRAYSIZE * (double)iters / (omp_get_wtime() - t_not);
	}
	
	//print out NUMPAD, peak MegaAdds/s, average MegaAdds/s
	std::fprintf(datafile, "%d, %.2lf, %.2lf\n", NUMPAD, peak/1000000., average/(1000000.*ave_iters));
	//print out 15 more data points for FIX 2 to make graphing easy	
	if(FIX == 2)
		for(int p = 0; p < 15; p++)
			std::fprintf(datafile, "%d, %.2lf, %.2lf\n", NUMPAD, peak/1000000., average/(1000000.*ave_iters));
	std::fclose(datafile);
			
	return 0;	
}


float rand_float(unsigned int *seedp, float low, float high) {

	float r = (float) rand_r(seedp);

	return(low + r * (high - low) / (float) RAND_MAX);
}


