#include <stdio.h>
#include <omp.h>

int main() 
{
    #pragma omp parallel num_threads(8)
    {
        printf("Hello world from thread %d of %d\n", 
            omp_get_thread_num(), omp_get_num_threads());
    }

    return 0;
}
