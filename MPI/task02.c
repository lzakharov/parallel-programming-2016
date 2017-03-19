#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100

void print(int *a, int size) {
    for (int i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int find_max(int *v, int size) {
    int max = v[0];

    for (int i = 1; i < size; i++) {
        if (v[i] > max) { max = v[i]; }
    }

    return max;
}

int main(int argc, char** argv) {
    srand(time(NULL));

    int world_size, rank;
    int *v = malloc(N * sizeof(int));
    int max;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunk_size = N / world_size;
    int remainder = N % world_size;

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            v[i] = (rand() % N) - (N / 2);
        }

        print(v, N);

        for (int i = 1; i < world_size; i++) {
            if (i < remainder) {
                MPI_Send(&v[i * chunk_size + 1], chunk_size + 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            } else {
                MPI_Send(&v[i * chunk_size], chunk_size, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        chunk_size += (remainder > 0 ? 1 : 0);
    } else {
        chunk_size += (rank < remainder + 1 ? 1 : 0);
        MPI_Recv(v, chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int local_max = find_max(v, chunk_size);

    if (rank == 0) {
        max = local_max;

        for (int i = 1; i < world_size; i++) {
            MPI_Recv(&local_max, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            if (local_max > max) { max = local_max; }
        }

        printf("Max: %d\n", max);
    } else {
        MPI_Send(&local_max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    free(v);

    MPI_Finalize();
}