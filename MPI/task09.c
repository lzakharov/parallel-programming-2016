#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_INT 1000
#define N 10

void print(int *a, int size) 
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void reverse(int *a, int size) 
{
    int k;

    for (int i = 0; i < size / 2; i++) {
        k = a[i];
        a[i] = a[size - i - 1];
        a[size - i - 1] = k;
    }
}

int main(int argc, char** argv) 
{
    srand(time(NULL));

    int world_size, rank;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *a = malloc(N * sizeof(int));
    int chunk_size = N / world_size;
    int rem = N % world_size;

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            a[i] = rand() % MAX_INT;
        }

        printf("a: ");
        print(a, N);
    }

    int *displs = malloc(world_size * sizeof(int));
    int *scounts = malloc(world_size * sizeof(int));
    int *chunk = malloc(chunk_size * sizeof(int));
    int sum = 0;

    for (int i = 0; i < world_size; i++) {
        scounts[i] = chunk_size + (i < rem ? 1 : 0);
        displs[i] = sum;
        sum += scounts[i];
    }

    MPI_Scatterv(a, scounts, displs, MPI_INT, chunk, scounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    reverse(chunk, scounts[rank]);
    reverse(displs, world_size);

    MPI_Gatherv(chunk, scounts[rank], MPI_INT, a, scounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Reverted: ");
        print(a, N);
    }

    free(chunk);
    free(displs);
    free(scounts);
    free(a);

    MPI_Finalize();
}