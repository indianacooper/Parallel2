#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>
#include <pthread.h>

#define MAXN 200
volatile float A[MAXN][MAXN], B[MAXN][MAXN], C[MAXN][MAXN];
static double mysecond()
{
        struct timeval  tp;
        struct timezone tzp;
        int i = 0;

        i = gettimeofday(&tp, &tzp);
        return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}

int main() {
    int i,j,k,stime;
    long ltime;

  /* get the current calendar time */
    ltime = time(NULL);
    stime = (unsigned) ltime/2;
    // Seed the random
    srand(stime);
    double start = 0.;
    double end = 0.;
//DEFINE STATIC MATRIX FOR TESTING

    for (i = 0; i< MAXN; i++) {
        for (j = 0; j < MAXN; j++) {
            A[i][j] = rand() % (10);
            B[i][j] = rand() % (10);
        }
  }
    const int dim = MAXN;
// BEGINNING OF Parallel STRUCTURE
    start = mysecond();
#pragma omp parallel num_threads(8)
#pragma omp for schedule(static)
    for (i = 0; i < dim; i++) {
        //#pragma omp parallel num_threads(8)
        //#pragma omp for schedule(static)
        for (j = 0; j < dim; j++) {
            C[i][j] = 0;
            //#pragma omp parallel num_threads(8)
            //#pragma omp for schedule(static)
            for (k = 0; k < dim; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    end = mysecond();
    for(i = 0; i< dim; i++){
        for(j = 0; j < dim; j++) {
            printf("%.1f ",C[i][j]);
        }
        printf("\n");
    }
    printf("Total time: %f\n", + (end - start));
}
