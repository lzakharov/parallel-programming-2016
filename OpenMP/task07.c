#include <stdio.h>
#include <omp.h>
#include "time.h"
#include "stdlib.h"

#define N 12

void print(int a[], int n) {
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int main() 
{
    int a[N], b[N], c[N];

    srand(time(NULL));

    #pragma omp parallel for schedule(static, 2) num_threads(3)
    for (int i = 0; i < N; i++)
    {
        a[i] = rand() % N;
        b[i] = rand() % N;
        printf("from thread %d of %d threads\n", 
            omp_get_thread_num(), omp_get_num_threads());
    }

    print(a, N);
    print(b, N);

    #pragma omp parallel for schedule(dynamic, 2) num_threads(3)
    for (int i = 0; i < N; i++)
    {
        c[i] = a[i] + b[i];
        printf("from thread %d of %d threads\n", 
            omp_get_thread_num(), omp_get_num_threads());
    }

    print(c, N);

    return 0;
}
