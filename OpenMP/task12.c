#include <stdio.h>
#include <omp.h>
#include "time.h"
#include "stdlib.h"

#define N 30 

int main() 
{
    int a[N];
    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        a[i] = rand() % (N);
        printf("%d ", a[i]);
    }

    int max = -1;

    #pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        if (a[i] % 7 == 0) {
            #pragma omp critical
            if (a[i] > max) { max = a[i]; }
        }
    }

    printf("Max number multiples of 7 is %d\n", max);

    return 0;
}