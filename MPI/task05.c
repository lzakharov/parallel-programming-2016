#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define CHUNK_SIZE 20000

void print(int *a, int size) {
    for (int i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int main(int argc, char** argv) {
    srand(time(NULL));

    int world_size, rank;
    int *a = malloc(N * sizeof(int));
    int *b = malloc(N * sizeof(int));
    int res;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunk_size = N / world_size;
    int remainder = N % world_size;

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            a[i] = (rand() % N) - (N / 2);
            b[i] = (rand() % N) - (N / 2);
        }

        printf("a: ");
        print(a, N);
        printf("b: ");
        print(b, N);
    }

    int *displs = malloc(world_size * sizeof(int));
    int *scounts = malloc(world_size * sizeof(int));
    int *sub_a = malloc(chunk_size * sizeof(int));
    int *sub_b = malloc(chunk_size * sizeof(int));
    int sum = 0;

    for (int i = 0; i < world_size; i++) {
        scounts[i] = chunk_size + (i < remainder ? 1 : 0);
        displs[i] = sum;
        sum += scounts[i];
    }

    MPI_Scatterv(a, scounts, displs, MPI_INT, sub_a, scounts[rank], MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(b, scounts, displs, MPI_INT, sub_b, scounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    int sub_res = 0;

    for (int i = 0; i < scounts[rank]; i++) {
        sub_res += sub_a[i] * sub_b[i];
    }

    MPI_Reduce(&sub_res, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Scalar product = %d\n", res);
    }

    free(a);
    free(b);
    free(displs);
    free(scounts);
    free(sub_a);
    free(sub_b);

    MPI_Finalize();
}