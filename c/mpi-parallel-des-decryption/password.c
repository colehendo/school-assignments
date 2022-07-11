#include <fcntl.h>
#include <fcntl.hLI>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <openssl/des.h>
#include <mpi.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

/*
 *
 */
int main(int argc, char **argv) {
    int world_size, world_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    double start, end, minStart, maxEnd;
    int count, foundPass, foundPassAny, i, j, x, y, z;
    count = foundPass = 0;
    start = MPI_Wtime();
    for (i = 33; i < 127; i++){
        for (j = 33; j < 127; j++){
            for (x = 33; x < 127; x++){
                for (y = 33; y < 127; y++){
                    for (z = 33; z < 127; z++){
                        if ((count % world_size) == world_rank) {
                            char pass[] = {i, j, x, y, z, '\0'};
                            char *cpass = DES_crypt(pass, "H2");
                            if (strcmp(cpass, "H2HHDpwTkPiLI") == 0){
                                printf("count: %d\n", count);
                                printf("pass: %s\n", pass);
                                foundPass = 1;
                            }
                        }
                    }
                    count++;
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce(&foundPass, &foundPassAny, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        if (foundPassAny != 0) {
            i = j = x = y = z = 0x7f;
            break;
        }
    }

    end = MPI_Wtime();
    MPI_Reduce(&start, &minStart, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&end, &maxEnd, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Finalize();
    if (end == maxEnd){
        printf("time: %f\n", (maxEnd - minStart));
    }
}
