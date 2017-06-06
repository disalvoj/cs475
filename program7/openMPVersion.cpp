/*
Author: Joshua Kluthe
Email: kluthej@oregonstate.edu
*/

#include <cstdlib>
#include <omp.h>
#include <iostream>
#include <cstdio>

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

    autocorrelate(array, sums, size, 1, "omp_1_thread.csv");
    autocorrelate(array, sums, size, 8, "omp_8_thread.csv");

    return 0;
}


//uses OpenMP to autocorrelate with the specified number of threads, and writes
//the collected data to a file named by param data_file
void autocorrelate(float *array, float *sums, int size, int threads, std::string data_file) {

    FILE *fp = std::fopen(data_file.c_str(), "w");    

    omp_set_num_threads(threads);

    int ave_loops = 10;
    double ave_time = 0;
    double peak_time = 1000;

    for(int loops = 0; loops < ave_loops; loops++) {
        
        double t_not = omp_get_wtime();

        #pragma omp parallel for default(none) shared(size, array, sums)
        for(int shift = 0; shift < size; shift++) {
	        float sum = 0.;
	        for(int i = 0; i < size; i++) {
		        sum += array[i] * array[i + shift];
	        }
	        sums[shift] = sum;
        }

        double t = omp_get_wtime();
        if(t - t_not < peak_time)
            peak_time = t - t_not;
        ave_time += t - t_not;
        
    }
    ave_time /= ave_loops;
    
    std::fprintf(fp, "Threads,Peak Performance (Mega Loops Per Second),"
                "Average Performance (Mega Loops Per Second)\n");
    std::fprintf(fp, "%d,%lf,%lf\n", threads, (double)size * size / peak_time / 1000000,
                (double)size * size / ave_time / 1000000);
    //write out the autocorrelated sums
    std::fprintf(fp, "Index,Sum\n");
    for(int i = 0; i < size; i++)
        std::fprintf(fp, "%d,%f\n", i, sums[i]);

    std::fclose(fp);
}


