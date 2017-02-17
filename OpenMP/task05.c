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

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            int sum = 0;

            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < M; j++)
                {
                    sum += a[i][j];
                }
            }

            float avg = (float)sum / (N * M);

            printf("Average: %.2f from thread %d\n", avg, omp_get_thread_num());
        }

        #pragma omp section
        {
            int min = N * M;
            int max = 0;

            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < M; j++)
                {
                    if (min > a[i][j]) { min = a[i][j]; }
                    if (max < a[i][j]) { max = a[i][j]; }
                }
            }

            printf("min=%d, max=%d from thread %d\n", 
                min, max, omp_get_thread_num());
        }

        #pragma omp section
        {
            int cnt = 0;
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < M; j++)
                {
                    if (a[i][j] % 3 == 0) { cnt++; }
                }
            }

            printf("Number of multiples of 3 is %d from thread %d\n", 
                cnt, omp_get_thread_num());
        }
    }

    return 0;
}