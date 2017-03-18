#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int size, rank;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processor_name, &name_len);

    printf("Hello world from processor %s, rank %d"
           " out of %d processors\n",
           processor_name, rank, size);

    MPI_Finalize();
}