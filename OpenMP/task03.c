#include <stdio.h>
#include <omp.h>

int main() 
{
    int a = 0, b = 0;
    printf("Before first section a=%d b=%d\n", a, b);

    #pragma omp parallel num_threads(2) private(a) firstprivate(b)
    {
        a = 0;
        a += omp_get_thread_num();
        b += omp_get_thread_num();

        printf("thread %d: a=%d b=%d\n", omp_get_thread_num(), a, b);
    }

    printf("After first section a=%d b=%d\n", a, b);

    printf("Before second section a=%d b=%d\n", a, b);

    #pragma omp parallel num_threads(2) shared(a) private(b)
    {
        b = 0;
        a -= omp_get_thread_num();
        b -= omp_get_thread_num();

        printf("thread %d: a=%d b=%d\n", omp_get_thread_num(), a, b);
    }

    printf("After second section a=%d b=%d\n", a, b);

    return 0;
}