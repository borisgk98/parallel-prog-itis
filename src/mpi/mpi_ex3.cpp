//
// Created by boris on 19.11.19.
//

#include <iostream>
#include <vector>
#include "/opt/mpich2/include/mpi.h"
using namespace std;

void mpi_ex3() {
    int rank, num;
    MPI_Status Status;

    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(MPI_COMM_WORLD,&num);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    const long number_of_iter = 10e8;
    long in = 0;
    long step = number_of_iter / num;
    for (int i = step * rank; i < step * (rank + 1) + (num == rank + 1 ? number_of_iter % num : 0); i++) {
        long x, y;
        x = rand();
        y = rand();
        if (x * x + y * y <= (long) RAND_MAX * RAND_MAX) {
            in++;
        }
    }
    if (rank == 0 ) {
        int sum = in;
        for (int i=1; i < num; i++ ) {
            MPI_Recv(&in, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Status);
            sum += in;
        }
        printf("PI = %f\n", (double) sum / number_of_iter * 4);
    }
    else {
        MPI_Send(&in, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}