#include <stdio.h>
#include <omp.h>

int main() 
{
    int nthreads, tid;

    #pragma omp parallel private(tid, nthreads)
    {
        tid = omp_get_thread_num();
        nthreads = omp_get_num_threads();
        printf("Hello world from thread %d of %d\n", tid, nthreads);
    }

    return 0;
}
