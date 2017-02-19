#include <stdio.h>
#include <omp.h>
#include "time.h"
#include "stdlib.h"

#define N 1000
#define M 1000

int main() 
{
    int a[N][M];
    int v[M];
    int res[N];
    double start;
    double end;
    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            a[i][j] = rand() / (N * M);
        }
    }

    for (int i = 0; i < M; i++)
    {
        v[i] = rand() / (N * M);
    }

    start = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            res[i] += a[i][j] * v[j];
        }
    }
    end = omp_get_wtime();

    printf("Single thread: %.6f\n", end - start);

    start = omp_get_wtime();
    #pragma omp parallel for schedule(auto) collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            res[i] += a[i][j] * v[j];
        }
    }
    end = omp_get_wtime();

    printf("Parallel: %.6f\n", end - start);

    return 0;
}