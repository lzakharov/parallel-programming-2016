#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define CHUNK_SIZE 200000

int main(int argc, char** argv) {
    srand(time(NULL));

    int world_size, rank;
    int in = 0, total_in = 0;
    double pi;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double *points = malloc(CHUNK_SIZE * sizeof(double));

    if (rank == 0) {
        for (int i = 1; i < world_size; i++) {
            for (int j = 0; j < CHUNK_SIZE; j++) {
                points[j] = (double)rand() * 2 / (double)RAND_MAX - 1;
            }

            MPI_Send(points, CHUNK_SIZE, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(points, CHUNK_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

        for (int i = 0; i < CHUNK_SIZE; i += 2) {
            double x = points[i];
            double y = points[i + 1];

            if (x * x + y * y <= 1) {
                in++;
            }
        }
    }

    MPI_Reduce(&in, &total_in, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        pi = 4.0 * total_in / ((CHUNK_SIZE / 2) * (world_size - 1));

        printf("%.2f\n", pi);
    }

    MPI_Finalize();
}