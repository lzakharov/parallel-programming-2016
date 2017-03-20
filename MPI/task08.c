#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INT 1000
#define N 20

void print(int *a, int size) 
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", a[i]);
    }

    printf("\n");
}

void scatter(void *sendbuf, int sendcount, MPI_Datatype sendtype, 
             void *recvbuf, int recvcount, MPI_Datatype recvtype, 
             int root, MPI_Comm comm) 
{
    int world_size, rank;
    int sendtype_size;
    MPI_Type_size(sendtype, &sendtype_size);

    MPI_Status status;
    MPI_Comm_size(comm, &world_size);
    MPI_Comm_rank(comm, &rank);

    if (rank == root) {
        for (int proc = 0; proc < world_size; proc++) {
            if (proc != root) {
                MPI_Send(sendbuf + proc * sendcount * sendtype_size, sendcount, sendtype, proc, 0, comm);
            } else {
                for (int i = 0; i < sendcount; i++) {
                    memcpy(recvbuf, sendbuf, sendcount * sendtype_size);
                }
            }
        }
    } else {
        MPI_Recv(recvbuf, recvcount, recvtype, root, 0, comm, &status);
    }
}

void gather(void *sendbuf, int sendcount, MPI_Datatype sendtype,
            void *recvbuf, int recvcount, MPI_Datatype recvtype,
            int root, MPI_Comm comm) 
{
    int world_size, rank;
    int recvtype_size;
    MPI_Type_size(recvtype, &recvtype_size);

    MPI_Status status;
    MPI_Comm_size(comm, &world_size);
    MPI_Comm_rank(comm, &rank);

    if (rank == root) {
        void *buf = malloc(recvcount * recvtype_size);

        for (int proc = 0; proc < world_size; proc++) {
            if (proc != root) {
                MPI_Recv(buf, recvcount, recvtype, proc, 0, comm, &status);
                memcpy((char *)recvbuf + proc * recvcount * recvtype_size, buf, recvcount * recvtype_size);
            } else {
                memcpy((char *)recvbuf + proc * recvcount * recvtype_size, sendbuf, recvcount * recvtype_size);
            }

        }

        free(buf);
    } else {
        MPI_Send(sendbuf, sendcount, sendtype, 0, 0, comm);
    }
}

int main(int argc, char** argv) 
{
    srand(time(NULL));
    int world_size, rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *a;
    int *new_a;

    if (rank == 0) {
        a = malloc(N * sizeof(int));

        for (int i = 0; i < N; i++) {
            a[i] = rand() % MAX_INT;
        }

        print(a, N);

        new_a = malloc(N * sizeof(int));
    }

    int chunk_size = N / world_size;
    int *chunk = malloc(chunk_size * sizeof(int));

    scatter(a, chunk_size, MPI_INT, chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    printf("rank %d: ", rank);
    print((int *)chunk, chunk_size);

    gather(chunk, chunk_size, MPI_INT, new_a, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        print((int *)new_a, N);
    }

    MPI_Finalize();
}