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

    int cnt = 0;

    #pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        if (a[i] % 9 == 0) {
            #pragma omp atomic
            cnt++;
        }
    }

    printf("Number of multiples of 9 is %d\n", cnt);

    return 0;
}