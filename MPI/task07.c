#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_INT 10
#define N 3 
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

void sum(int *a, int *b, int size) {
    for (int i = 0; i < size; i++) {
        a[i] += b[i];
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));

    int world_size, rank;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *v;
    int *chunk;
    int chunk_rows = N;
    int chunk_columns;
    int chunk_size;
    int rem = M % world_size;

    if (rank == 0) {
        int *a = malloc(N * M * sizeof(int));
        v = malloc(M * sizeof(int));

        for (int j = 0; j < M; j++) {
            for (int i = 0; i < N; i++) {
                a[i * M + j] = rand() % MAX_INT;
            }

            v[j] = rand() % MAX_INT;
        }

        print(a, N, M);
        printf("------------------------------------\n");
        print(v, M, 1);
        printf("------------------------------------\n");

        int bias = M / world_size + (rem > 0 ? 1 : 0);

        int *tmp = malloc(chunk_rows * (M / world_size + 1) * sizeof(int));

        for (int proc = 1; proc < world_size; proc++) {
            chunk_columns = M / world_size + (proc < rem ? 1 : 0);
            chunk_size = chunk_rows * chunk_columns;

            for (int i = 0; i < chunk_rows; i++) {
                for (int j = 0; j < chunk_columns; j++) {
                    tmp[i * chunk_columns + j] = a[i * M + j + bias];
                }
            }

            MPI_Send(tmp, chunk_size, MPI_INT, proc, 0, MPI_COMM_WORLD);
            MPI_Send(&v[bias], chunk_columns, MPI_INT, proc, 1, MPI_COMM_WORLD);
            bias += chunk_columns;
        }

        free(tmp);

        chunk_columns = M / world_size + (rem > 0 ? 1 : 0);
        chunk_size = chunk_rows * chunk_columns;
        chunk = malloc(chunk_size * sizeof(int));
        for (int i = 0; i < chunk_rows; i++) {
            for (int j = 0; j < chunk_columns; j++) {
                chunk[i * chunk_columns + j] = a[i * M + j];
            }
        }
    } else {
        chunk_columns = M / world_size + (rank < rem ? 1 : 0);
        chunk_size = chunk_rows * chunk_columns;
        chunk = malloc(chunk_size * sizeof(int));
        v = malloc(chunk_columns * sizeof(int));

        MPI_Recv(chunk, chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(v, chunk_columns, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }

    int *p = malloc(N * sizeof(int));

    for (int i = 0; i < N; i++) { p[i] = 0; };

    for (int i = 0; i < chunk_rows; i++) {
        for (int j = 0; j < chunk_columns; j++) {
            p[i] += chunk[i * chunk_columns + j] * v[j];
        }
    }

    if (rank == 0) {
        int *res = malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) { res[i] = 0; };
        sum(res, p, N);

        for (int i = 1; i < world_size; i++) {
            MPI_Recv(p, N, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            sum(res, p, N);
        }

        print(res, N, 1);
        free(res);
    } else {
        MPI_Send(p, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    free(p);
    free(chunk);
    free(v);

    MPI_Finalize();
}