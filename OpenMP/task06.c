#include <stdio.h>
#include <omp.h>
#include "time.h"
#include "stdlib.h"

#define N 10

void print(int a[], int n) {
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int main() 
{
    int a[N], b[N];

    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        a[i] = rand() % N;
        b[i] = rand() % N;
    }

    print(a, N);
    print(b, N);

    int sum_a = 0;
    int sum_b = 0;

    #pragma omp parallel for reduction(+: sum_a) reduction(+: sum_b)
    for (int i = 0; i < N; i++)
    {
        sum_a += a[i];
        sum_b += b[i];
    }

    float avg_a = (float)sum_a / N;
    float avg_b = (float)sum_b / N;

    printf("Average of first array: %.2f\n", avg_a);
    printf("Average of second array: %.2f\n", avg_b);

    return 0;
}
