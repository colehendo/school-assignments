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
    double byte_count = msg_count * sizeof(double);
    int world_size, world_rank, i;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    double sendArray[32768], localArray[32768], start, end;
    MPI_Request send_request, recv_request;
    MPI_Status status;

    for (i = 0; i < msg_count; i++) {
        sendArray[i] = 4;
    }

    if (world_rank == 0) {
        start = MPI_Wtime();
        MPI_Isend(&sendArray, msg_count, MPI_DOUBLE, (world_rank + 1), world_rank, MPI_COMM_WORLD, &send_request);
        MPI_Irecv(&localArray, msg_count, MPI_DOUBLE, (world_size - 1), (world_size - 1), MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
        end = MPI_Wtime();
        printf("ring time for %d count: %f\n", msg_count, ((end - start) * 1000000));
        printf("Throughput: %d\n", (byte_count / (end - start)));
    }

    else if (world_rank == (world_size - 1)) {
        MPI_Irecv(&localArray, msg_count, MPI_DOUBLE, (world_rank - 1), (world_rank - 1), MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
        MPI_Isend(&localArray, msg_count, MPI_DOUBLE, 0, world_rank, MPI_COMM_WORLD, &send_request);
    }

    else {
        MPI_Irecv(&localArray, msg_count, MPI_DOUBLE, (world_rank - 1), (world_rank - 1), MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
        MPI_Isend(&localArray, msg_count, MPI_DOUBLE, (world_rank + 1), world_rank, MPI_COMM_WORLD, &send_request);
    }

    MPI_Finalize();
}
