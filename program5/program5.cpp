/*
 * Author: Joshua Kluthe
 * Date: 2017.05.21
 * 
 * Description: This program compare SIMD SSE multiplication to normal C++
 * multiplication, and SIMD SSE multiplication/reduction to normal C++ style
 * multiplication/reduction. The array size and test type are expected to be
 * defined in the initial compiler command, but all of this is taken care of in
 * the run_prog5.py script so you don't have to worry about it.
 * 
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include "simd.p5.h"

float A[ARRAY_SIZE];
float B[ARRAY_SIZE];
float C[ARRAY_SIZE];

float rand_float(unsigned int *seedp, float low, float high);
void fill_array(float *arry, int size);

void mul(float *A, float *B, float *C, int len);
float mul_sum(float *A, float *B, int len);

//ARRAY_SIZE is the size of the arrays used for the multiplication/reduction
//TEST determines which comparisons are being made
//TEST == 0: SIMD SSE multiplication vs C++ multiplication
//TEST == 1: SIMD SSE mult + reduction vs C++ mult + reduction.
//argv[1] = name of file to append data to
//argv[2] = name of file to append speedup data to
int main(int argc, char *argv[]) {
	
#ifndef _OPENMP
    std::fprintf(stderr, "OpenMP not supported, exiting program.\n");
    return 1;
#endif

	//open data files
	FILE *datafile = std::fopen(argv[1], "a");
	FILE *speedup_data = std::fopen(argv[2], "a");

	fill_array(A, ARRAY_SIZE);
	fill_array(B, ARRAY_SIZE);

	float cpp_ave_time = 0;
	float cpp_peak_time = 1000000;
	float sse_ave_time = 0;
	float sse_peak_time = 1000000;

	const int loops_for_ave = 200;

	double t_not;
	double t;

	if(TEST == 0) {

		//run trials to average the times for C++
		for(int i = 0; i < loops_for_ave; i++) {

			//time the C++ multiplication loop
			t_not = omp_get_wtime();
            mul(A, B, C, ARRAY_SIZE);
			t = omp_get_wtime();
			
			cpp_ave_time += t - t_not;
			if(t - t_not < cpp_peak_time)
				cpp_peak_time = t - t_not; 

		}
		cpp_ave_time /= loops_for_ave;

		//run trials to average the times for SIMD SSE
		for(int i = 0; i < loops_for_ave; i++) {

			//time the SIMD SSE multiplication loop
			t_not = omp_get_wtime();
			SimdMul(A, B, C, ARRAY_SIZE);
			t = omp_get_wtime();
			
			sse_ave_time += t - t_not;
			if(t - t_not < sse_peak_time)
				sse_peak_time = t - t_not; 

		}
		sse_ave_time /= loops_for_ave;

	} else {

		//run trials to average the times for C++
		for(int i = 0; i < loops_for_ave; i++) {

			//time the C++ multiplication/reduction loop
			t_not = omp_get_wtime();
            float sum = mul_sum(A, B, ARRAY_SIZE);
			t = omp_get_wtime();
			
			cpp_ave_time += t - t_not;
			if(t - t_not < cpp_peak_time)
				cpp_peak_time = t - t_not; 

		}
		cpp_ave_time /= loops_for_ave;

		//run trials to average the times for SIMD SSE
		for(int i = 0; i < loops_for_ave; i++) {

			//time the SIMD SSE multiplication loop
			t_not = omp_get_wtime();
			float sum = SimdMulSum(A, B, ARRAY_SIZE);
			t = omp_get_wtime();
			
			sse_ave_time += t - t_not;
			if(t - t_not < sse_peak_time)
				sse_peak_time = t - t_not; 

		}
		sse_ave_time /= loops_for_ave;

	}

	std::fprintf(datafile, "Normal C++ Data:\n"
		"Array Size,Peak Speed,Average Speed\n"
		"%d,%.8f,%.8f\n", ARRAY_SIZE, cpp_peak_time, cpp_ave_time);

	std::fprintf(datafile, "SIMD SSE Data:\n"
		"Array Size,Peak Speed,Average Speed\n"
		"%d,%.8f,%.8f\n", ARRAY_SIZE, sse_peak_time, sse_ave_time);
	std::fprintf(datafile, "Speedup Data (C++ Speed / SIMD SSE Speed):\n"
		"Array Size,Peak Speedup,Average Speedup\n"
		"%d,%.8f,%.8f\n", ARRAY_SIZE, cpp_peak_time / sse_peak_time, cpp_ave_time / sse_ave_time);

    std::fprintf(speedup_data, "%d,%.8f,%.8f\n", ARRAY_SIZE, cpp_peak_time / sse_peak_time, cpp_ave_time / sse_ave_time);

	std::fclose(datafile);

	std::fclose(speedup_data);			

	return 0;	
}

void mul(float *A, float *B, float *C, int len) {

    for(int i = 0; i < len; i++) {

		C[i] = A[i]*B[i];

	}

}

float mul_sum(float *A, float *B, int len) {

    float sum = 0;
	for(int i = 0; i < len; i++) {
		sum += A[i]*B[i];
	}
    return sum;
}

void fill_array(float *arry, int size) {
	
	unsigned int seed = 0;

	for(int i = 0; i < size; i++)
		arry[i] = rand_float(&seed, -1., 1.);

}

float rand_float(unsigned int *seedp, float low, float high) {

	float r = (float) rand_r(seedp);

	return(low + r * (high - low) / (float) RAND_MAX);
}


