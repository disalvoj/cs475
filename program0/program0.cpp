#include <omp.h>
#include <cstdio>
#include <cmath>
#include <cstdlib>

#define ARRAYSIZE 100000
#define NUMTRIES 100000

void fill_array(float *arry, int size);
double average_mega_mults(const float *A, const float *B, int threads);

int main(void) {
    
#ifndef _OPENMP
    std::fprintf(stderr, "OpenMP not supported, exiting program.\n");
    return 1;
#endif

    float A[ARRAYSIZE];
    float B[ARRAYSIZE];

    fill_array(A, ARRAYSIZE);
    fill_array(B, ARRAYSIZE);

	double ave_1_threads = average_mega_mults(A, B, 1);
	double ave_4_threads = average_mega_mults(A, B, 4);

    return 0;
}


void fill_array(float *arry, int size) {

    std::srand(omp_get_wtime());

    for(int i = 0; i < size; i++)
        arry[i] = (double) (std::rand() % 10000);

}


double average_mega_mults(const float *A, const float *B, int threads) {

    float C[ARRAYSIZE];

    omp_set_num_threads(threads);

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
    std::printf("\nUsing %d threads.\n", threads);
    std::printf("Best run: %8.2lf MegaMults/s\n", max);
    std::printf("Average: %8.2lf MegaMults/s\n", ave);

	return ave;
}


