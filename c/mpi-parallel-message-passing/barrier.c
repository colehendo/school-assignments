#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <mpi.h>

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"


void ring_barrier(int world_rank, int world_size) {
    double send, local_var;
    MPI_Request send_request, recv_request, recv_request2;

    send = 4;

    if (world_rank == 0) {
        MPI_Isend(&send, 1, MPI_DOUBLE, 1, world_rank, MPI_COMM_WORLD, &send_request);
        MPI_Irecv(&local_var, 1, MPI_DOUBLE, (world_size - 1), (world_size - 1), MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
        MPI_Isend(&send, 1, MPI_DOUBLE, 1, world_rank, MPI_COMM_WORLD, &send_request);
        MPI_Irecv(&local_var, 1, MPI_DOUBLE, (world_size - 1), (world_size - 1), MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
    }

    else if (world_rank == (world_size - 1)) {
        MPI_Irecv(&local_var, 1, MPI_DOUBLE, (world_rank - 1), (world_rank - 1), MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
        MPI_Isend(&local_var, 1, MPI_DOUBLE, 0, world_rank, MPI_COMM_WORLD, &send_request);
        MPI_Irecv(&local_var, 1, MPI_DOUBLE, (world_rank - 1), (world_rank - 1), MPI_COMM_WORLD, &recv_request2);
        MPI_Wait(&recv_request2, MPI_STATUS_IGNORE);
        MPI_Isend(&local_var, 1, MPI_DOUBLE, 0, world_rank, MPI_COMM_WORLD, &send_request);
    }

    else {
        MPI_Irecv(&local_var, 1, MPI_DOUBLE, (world_rank - 1), (world_rank - 1), MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
        MPI_Isend(&local_var, 1, MPI_DOUBLE, (world_rank + 1), world_rank, MPI_COMM_WORLD, &send_request);
        MPI_Irecv(&local_var, 1, MPI_DOUBLE, (world_rank - 1), (world_rank - 1), MPI_COMM_WORLD, &recv_request2);
        MPI_Wait(&recv_request2, MPI_STATUS_IGNORE);
        MPI_Isend(&local_var, 1, MPI_DOUBLE, (world_rank + 1), world_rank, MPI_COMM_WORLD, &send_request);
    }
}

int main(int argc, char **argv) {
    int world_size, world_rank, i;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    double start, end;

    start = MPI_Wtime();

    for (i = 0; i < 1000; i++) {
        ring_barrier(world_rank, world_size);
    }

    end = MPI_Wtime();

    if (world_rank == 0) {
        printf("time for 1000 ring barriers: %f\n", ((end - start) * 1000000));
        printf("ring throughput is: %f\n", (sizeof(double) / ((end - start) * 10000000)));
    }

    MPI_Finalize();
}
