#include <stdio.h>
#include <omp.h>

int main() {
    int nthreads;
    nthreads = 3;

    #pragma omp parallel if(nthreads > 1) num_threads(nthreads)
    {
        if (omp_in_parallel()) {
            printf("Hello world from thread %d of %d\n", 
                omp_get_thread_num(), omp_get_num_threads());
        }
    }

    nthreads = 1;

    #pragma omp parallel if(nthreads > 1) num_threads(nthreads)
    {
        if (omp_in_parallel()) {
            printf("Hello world from thread %d of %d\n", 
                omp_get_thread_num(), omp_get_num_threads());
        }
    }

    return 0;
}
