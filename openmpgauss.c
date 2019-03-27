#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>
#include <pthread.h>

#define MAXN 2000
volatile float A[MAXN][MAXN], B[MAXN], X[MAXN];
static double mysecond()
{
        struct timeval  tp;
        struct timezone tzp;
        int i = 0;

        i = gettimeofday(&tp, &tzp);
        return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}

int main() {
    int i,j,k,stime,nt;
    int norm, row, col;
    long ltime;
    printf("Please enter 1,2,4, or 8 threads:");
    scanf("%d",&nt);
  /* get the current calendar time */
    ltime = time(NULL);
    stime = (unsigned) ltime/2;
    // Seed the random
    srand(stime);
    double start = 0.;
    double end = 0.;
//DEFINE STATIC MATRIX FOR TESTING

    for (col = 0; col < MAXN; col++) {
        for (row = 0; row < MAXN; row++) {
            A[row][col] = (float)rand() / 32768.0;
        }
        B[col] = (float)rand() / 32768.0;
        X[col] = 0.0;
    }

    /* Normalization row, and zeroing
    * element row and col */
    float multiplier;
    /* Gaussian elimination */
    start = mysecond();
    #pragma omp parallel num_threads(nt)
    #pragma omp for schedule(static)
    for (norm = 0; norm < MAXN - 1; norm++)
    {
        for (row = norm + 1; row < MAXN; row++)
        {
            multiplier = A[row][norm] / A[norm][norm];
            for (col = norm; col < MAXN; col++)
            {

                A[row][col] -= A[norm][col] * multiplier;

            }
            B[row] -= B[norm] * multiplier;
        }
    }
/* (Diagonal elements are not normalized to 1. This is treated in back
* substitution.)
*/

/* Back substitution */
    for (row = MAXN - 1; row >= 0; row--)
    {
        X[row] = B[row];
        for (col = MAXN-1; col > row; col--)
        {
            X[row] -= A[row][col] * X[col];
        }
    X[row] /= A[row][row];
    }
    end = mysecond();
    printf("Total time: %f\n",(end - start));
    printf("Number of threads: %d\n",nt);
}


