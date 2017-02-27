#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int main() 
{
    int nthreads, tid;

    #pragma omp parallel num_threads(8) private(tid)
    {
        tid = omp_get_thread_num();
        nthreads = omp_get_num_threads();
        sleep(nthreads - tid);
        printf("Hello world from thread %d of %d\n", tid, nthreads);
    }

    int cnt = 7;

    #pragma omp parallel num_threads(8)
    {
        while (cnt > 0) {
            if (omp_get_thread_num() == cnt) {
                #pragma omp critical
                {
                    cnt--;
                    printf("Hello world from thread %d of %d\n", 
                        omp_get_thread_num(), omp_get_num_threads());
                }
            }
        }
    }

    return 0;
}
