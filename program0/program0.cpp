#include <omp.h>
#include <cstdio>
#include <cmath>
#include <cstdlib>

#define NUMTHREADS 4
#define ARRAYSIZE 100000
#define NUMTRIES 100

void fill_array(float *arry, int size);

int main(void) {
    
#ifndef _OPENMP
    std::fprintf(stderr, "OpenMP not supported, exiting program.\n");
    return 1;
#endif

    float A[ARRAYSIZE];
    float B[ARRAYSIZE];
    float C[ARRAYSIZE];

    fill_array(A, ARRAYSIZE);
    fill_array(B, ARRAYSIZE);

    omp_set_num_threads(NUMTHREADS);
    std::printf("Using %d threads.\n", NUMTHREADS);

    double max = 0;
    double sum = 0;

    for(int i = 0; i < NUMTRIES; i++) {
        double t_not = omp_get_wtime();
        #pragma omp parallel for
        for(int j = 0; j < ARRAYSIZE; j++) {
            C[j] = A[j] * B[j];
        }
        double t = omp_get_wtime();
        double megaMults = (double) ARRAYSIZE / (t - t_not) / 1000000.;
        sum += megaMults;
        if(megaMults > max)
            max = megaMults;

    }
    double ave = sum / (double) NUMTRIES;
    std::printf("Best run: %8.2lf MegaMults/s\n", max);
    std::printf("Average: %8.2lf MegaMults/s\n", ave);

    return 0;
}


void fill_array(float *arry, int size) {

    std::srand(omp_get_wtime());

    for(int i = 0; i < size; i++)
        arry[i] = (double) (std::rand() % 10000);

}


