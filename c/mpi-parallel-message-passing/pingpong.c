#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <mpi.h>

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"



int main(int argc, char **argv) {
    int msg_count = 1;
    double byte_count = msg_count * 8;
    int world_size, world_rank, i;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    double sendArray[32768], localArrayB[32768], localArrayNB[32768], localArrayNBS[32768], localArrayNBR[32768], start, end;
    MPI_Request send_request, recv_request;
    MPI_Status status;

    for (i = 0; i < msg_count; i++) {
        sendArray[i] = 4;
    }

    /* Blocking */
    if (world_rank == 0) {
        start = MPI_Wtime();
        MPI_Send(&sendArray, msg_count, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
        MPI_Recv(&localArrayB, msg_count, MPI_DOUBLE, 1, 2, MPI_COMM_WORLD, &status);
        end = MPI_Wtime();
        printf("Blocking, %d size, time: %f\n", msg_count, ((end - start) * 1000000));
        printf("Throughput: %f\n", (byte_count / ((end - start) * 1000000)));
    }

    else if (world_rank == 1) {
        MPI_Recv(&localArrayB, msg_count, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Send(&localArrayB, msg_count, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }

    /* Non-Blocking */
    if (world_rank == 0) {
        start = MPI_Wtime();
        MPI_Isend(&sendArray, msg_count, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, &send_request);
        MPI_Irecv(&localArrayNB, msg_count, MPI_DOUBLE, 1, 2, MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
        end = MPI_Wtime();
        printf("Non-blocking, %d size, time: %f\n", msg_count, ((end - start) * 1000000));
        printf("Throughput: %f\n", (byte_count / ((end - start) * 1000000)));
    }

    else if (world_rank == 1) {
        MPI_Isend(&localArrayNB, msg_count, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &send_request);
        MPI_Irecv(&localArrayNB, msg_count, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &recv_request);
    }

    /* NB Send */
    if (world_rank == 0) {
        start = MPI_Wtime();
        MPI_Isend(&sendArray, msg_count, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, &send_request);
        MPI_Recv(&localArrayNBS, msg_count, MPI_DOUBLE, 1, 2, MPI_COMM_WORLD, &status);
        end = MPI_Wtime();
        printf("NB Send, %d size, time: %f\n", msg_count, ((end - start) * 1000000));
        printf("Throughput: %f\n", (byte_count / ((end - start) * 1000000)));
    }

    else if (world_rank == 1) {
        MPI_Recv(&localArrayNBS, msg_count, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Isend(&localArrayNBS, msg_count, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &send_request);
    }

    /* NB Recv */
    if (world_rank == 0) {
        start = MPI_Wtime();
        MPI_Send(&sendArray, msg_count, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
        MPI_Irecv(&localArrayNBR, msg_count, MPI_DOUBLE, 1, 2, MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
        end = MPI_Wtime();
        printf("NB Recv, %d size, time: %f\n", msg_count, ((end - start) * 1000000));
        printf("Throughput: %f\n", (byte_count / ((end - start) * 1000000)));
    }

    else if (world_rank == 1) {
        MPI_Irecv(&localArrayNBR, msg_count, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
        MPI_Send(&localArrayNBR, msg_count, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
