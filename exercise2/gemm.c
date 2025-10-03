#define min(x,y) (((x) < (y)) ? (x) : (y))
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "cblas.h"

#ifdef USE_FLOAT
#define MYFLOAT float
#define GEMMCPU cblas_sgemm
#define PRECISION "float"
#endif

#ifdef USE_DOUBLE
#define MYFLOAT double
#define GEMMCPU cblas_dgemm
#define PRECISION "double"
#endif

#ifdef OPENBLAS
#define LIBRARY "openblas"
#endif

#ifdef BLIS
#define LIBRARY "blis"
#endif

struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

int main(int argc, char** argv)
{
    MYFLOAT *A, *B, *C;
    int m, n, k;
    MYFLOAT alpha, beta;
    struct timespec begin, end;
    double elapsed;
    FILE *fp;
    char filename[256];
    char *binding = getenv("OMP_PROC_BIND");
    char *cores_str = getenv("OMP_NUM_THREADS");
    int cores = cores_str ? atoi(cores_str) : 1;
    
    if (binding == NULL) binding = "unknown";

    if (argc == 1) {
        m = 2000, k = 200, n = 1000;
    } else if (argc == 4) {
        m = atoi(argv[1]);
        k = atoi(argv[2]);
        n = atoi(argv[3]);
    } else {
        printf("Usage: %s M K N\n", argv[0]);
        return 0;
    }

    alpha = 1.0;
    beta = 0.0;

    A = (MYFLOAT *)malloc(m*k*sizeof(MYFLOAT));
    B = (MYFLOAT *)malloc(k*n*sizeof(MYFLOAT));
    C = (MYFLOAT *)malloc(m*n*sizeof(MYFLOAT));

    if (A == NULL || B == NULL || C == NULL) {
        printf("ERROR: Memory allocation failed\n");
        free(A);
        free(B);
        free(C);
        return 1;
    }

    for (int i = 0; i < (m*k); i++) {
        A[i] = (MYFLOAT)(i+1);
    }
    for (int i = 0; i < (k*n); i++) {
        B[i] = (MYFLOAT)(-i-1);
    }
    for (int i = 0; i < (m*n); i++) {
        C[i] = 0.0;
    }

    sleep(1);

    clock_gettime(CLOCK_MONOTONIC, &begin);
    GEMMCPU(CblasColMajor, CblasNoTrans, CblasNoTrans,
            m, n, k, alpha, A, m, B, k, beta, C, m);
    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed = (double)diff(begin,end).tv_sec + (double)diff(begin,end).tv_nsec / 1000000000.0;
    double gflops = 2.0 * m * n * k / elapsed * 1.0e-9;

    printf("%dx%dx%d\t%lf\t%lf\t%s\t%s\t%d\n", m, n, k, elapsed, gflops, PRECISION, binding, cores);

    // Append to library-specific CSV file
    snprintf(filename, sizeof(filename), "%s.csv", LIBRARY);
    fp = fopen(filename, "a");
    if (fp != NULL) {
        fprintf(fp, "%d,%d,%d,%lf,%lf,%s,%s,%d\n", m, n, k, elapsed, gflops, PRECISION, binding, cores);
        fclose(fp);
    }

    free(A);
    free(B);
    free(C);
    return 0;
}