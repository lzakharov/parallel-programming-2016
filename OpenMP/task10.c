#include <stdio.h>
#include <omp.h>
#include "time.h"
#include "stdlib.h"

#define N 6
#define M 8

int main() 
{
    int a[N][M];
    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            a[i][j] = rand() % (N * M);
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }

    int min = N * M;
    int max = 0;

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            #pragma omp critical
            if (a[i][j] > max) { max = a[i][j]; }
            #pragma omp critical
            if (a[i][j] < min) { min = a[i][j]; }
        }
    }

    printf("min=%d, max=%d\n", min, max);

    return 0;
}