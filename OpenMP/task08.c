#include <stdio.h>
#include <omp.h>
#include "time.h"
#include "stdlib.h"

#define N 16000

int main() 
{
    int a[N];
    double b[N];

    for (int i = 0; i < N; i++)
    {
        a[i] = i;
    }

    double start = omp_get_wtime();
    #pragma omp parallel for schedule(static) shared(b) num_threads(8)
    for (int i = 1; i < N - 1; i++)
    {
        b[i] = (a[i - 1] + a[i] + a[i + 1]) * 1.0 / 3;
    }
    double end = omp_get_wtime();

    printf("Static: %.6fs\n", end - start);

    start = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic) shared(b) num_threads(8)
    for (int i = 1; i < N - 1; i++)
    {
        b[i] = (a[i - 1] + a[i] + a[i + 1]) * 1.0 / 3;
    }
    end = omp_get_wtime();

    printf("Dynamic with chunk=1: %.6fs\n", end - start);

    start = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, 2) shared(b) num_threads(8)
    for (int i = 1; i < N - 1; i++)
    {
        b[i] = (a[i - 1] + a[i] + a[i + 1]) * 1.0 / 3;
    }
    end = omp_get_wtime();

    printf("Dynamic with chunk=2: %.6fs\n", end - start);

    start = omp_get_wtime();
    #pragma omp parallel for schedule(guided) shared(b) num_threads(8)
    for (int i = 1; i < N - 1; i++)
    {
        b[i] = (a[i - 1] + a[i] + a[i + 1]) * 1.0 / 3;
    }
    end = omp_get_wtime();

    printf("Guided: %.6fs\n", end - start);

    start = omp_get_wtime();
    #pragma omp parallel for schedule(auto) shared(b) num_threads(8)
    for (int i = 1; i < N - 1; i++)
    {
        b[i] = (a[i - 1] + a[i] + a[i + 1]) * 1.0 / 3;
    }
    end = omp_get_wtime();

    printf("Auto: %.6fs\n", end - start);

    return 0;
}
