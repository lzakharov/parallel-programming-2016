#include <stdio.h>
#include <omp.h>
#include "time.h"
#include "stdlib.h"

const int N = 10;

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

    int min = 10, max = 0;

    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        {
            for (int i = 0; i < N; i++)
            {
                if (min > a[i]) { min = a[i]; }
            }
        }

        #pragma omp section
        {
            for (int i = 0; i < N; i++)
            {
                if (max < b[i]) { max = b[i]; }
            }
        }
    }


    printf("min=%d max=%d\n", min, max);

    return 0;
}