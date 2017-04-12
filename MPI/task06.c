#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_INT 1000
#define N 4
#define M 3 

void print(int *a, int rows, int columns) {
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++) {
            printf("%d ", a[i * columns + j]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));

    int world_size, rank;
    int *a = malloc(N * M * sizeof(int));
    int res = -1;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                a[i * M + j] = rand() % MAX_INT;
            }
        }

        print(a, N, M);
    }

    int *displs = malloc(world_size * sizeof(int));
    int *scounts = malloc(world_size * sizeof(int));
    int *chunk;
    int chunk_rows;
    int chunk_columns = M;
    int chunk_size;
    int rem = M % world_size;
    int bias = 0;

    for (int i = 0; i < world_size; i++) {
        chunk_rows = (N / world_size) + (i < rem ? 1 : 0);
        chunk_size = chunk_rows * chunk_columns;
        scounts[i] = chunk_size;
        displs[i] = bias;
        bias += scounts[i];
    }

    chunk_rows = (N / world_size) + (rank < rem ? 1 : 0);
    chunk_size = chunk_rows * chunk_columns;
    chunk = malloc(chunk_size * sizeof(int));

    MPI_Scatterv(a, scounts, displs, MPI_INT, chunk, scounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    int max = -1;

    for (int i = 0; i < chunk_rows; i++) {
        int min = chunk[i * chunk_columns];

        for (int j = 1; j < chunk_columns; j++) {
            if (min > chunk[i * chunk_columns + j]) { 
                min = chunk[i * chunk_columns + j];
            }
        }

        if (min > max) { max = min; }
    }

    if (rank == 0) {
        int res = max;

        for (int proc = 1; proc < world_size; proc++) {
            MPI_Recv(&max, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);

            if (max > res) { res = max; }
        }

        printf("maxmin = %d\n", res);
    } else {
        MPI_Send(&max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    free(chunk);
    free(scounts);
    free(displs);
    free(a);

    MPI_Finalize();
}