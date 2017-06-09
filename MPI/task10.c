#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define N 100000

int main(int argc, char** argv) {
    int size, rank;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int x[N];
    double start, end, total;

    srand(time(NULL));
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            x[i] = rand() % 100;
        }
    }

    //Send
    if (rank == 0) {
        start = MPI_Wtime();
        MPI_Send(x, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(x, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    
    
    if (rank == 0) {
        end = MPI_Wtime();
        total = end - start;

        printf("Send  %.6f\n", total);
    }
    
    //Ssend
    if (rank == 0) {
        start = MPI_Wtime();
        MPI_Ssend(x, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(x, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    
    if (rank == 0) {
        end = MPI_Wtime();
        total = end - start;

        printf("Ssend  %.6f\n", total);
    }

    //Rsend
    if (rank == 0) {
        start = MPI_Wtime();

        MPI_Rsend(x, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(x, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }


    if (rank == 0) {
        end = MPI_Wtime();
        total = end - start;

        printf("Rsend  %.6f\n", total);
    }

    //Bsend
    int bsize;
    int *buff;
    MPI_Buffer_attach(malloc(N * sizeof(int)), N * sizeof(int) + MPI_BSEND_OVERHEAD);

    if (rank == 0) {
        start = MPI_Wtime();
        MPI_Bsend(x, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(x, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }


    if (rank == 0) {
        end = MPI_Wtime();
        total = end - start;

        printf("Bsend  %.6f\n", total);
    }

    MPI_Buffer_detach(&buff, &bsize);
    MPI_Finalize();
}