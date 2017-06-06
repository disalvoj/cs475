/*
Author: Joshua Kluthe
Email: kluthej@oregonstate.edu
*/

#include <cstdlib>
#include <omp.h>
#include <iostream>
#include <cstdio>
#include "simd.p5.h"

void autocorrelate(float *array, float *sums, int size, int threads, std::string data_file);

int main() {

#ifndef _OPENMP
    std::fprintf(stderr, "OpenMP not supported, exiting program.\n");
    return 1;
#endif

    FILE *fp = std::fopen("signal.txt", "r");

    if(!fp)
        std::cout << "Failed to open file." << std::endl;

    int size;
    fscanf(fp, "%d", &size);

    float *array = new float[2 * size];
    float *sums  = new float[size];
    for(int i = 0; i < size; i++) {
    	std::fscanf(fp, "%f", &array[i]);
	    array[i + size] = array[i];
    }
    std::fclose(fp);

    int ave_loops = 10;
    double ave_time = 0;
    double peak_time = 1000000;
    double t_not, t;

	for(int i = 0; i < ave_loops; i++) {

		t_not = omp_get_wtime();
        for(int shift = 0; shift < size; shift++) {
		    sums[shift] = SimdMulSum(array, &array[shift], size);
        }
		t = omp_get_wtime();
		
		ave_time += t - t_not;
		if(t - t_not < peak_time)
			peak_time = t - t_not; 

	}

    ave_time /= ave_loops;

    fp = std::fopen("simd.csv", "w");

    std::fprintf(fp, "Peak Performance (Mega Loops Per Second),"
                "Average Performance (Mega Loops Per Second)\n");
    std::fprintf(fp, "%lf,%lf\n", (double)size * size / peak_time / 1000000,
                (double)size * size / ave_time / 1000000);
    //write out the autocorrelated sums
    std::fprintf(fp, "Index,Sum\n");
    for(int i = 0; i < size; i++)
        std::fprintf(fp, "%d,%f\n", i, sums[i]);

    std::fclose(fp);

    return 0;
}


