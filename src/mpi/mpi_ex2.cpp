//
// Created by boris on 09.11.2019.
//

#include <iostream>
#include <vector>
#include "/opt/mpich2/include/mpi.h"
using namespace std;

void mpi_ex2() {
    int rank, num, n=100;
    double x[n], maxn = 0.0, pmaxn = 0.0;
    MPI_Status mpi_status;

    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(MPI_COMM_WORLD,&num);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if (rank == 0 ) {
        for (int i = 0; i < n; i++) {
            x[i] = i;
        }
    }

    MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    int k = n / num;
    int i1 = k * rank;
    int i2 = k * (rank + 1 );
    if (rank == num - 1 ) {
        i2 = n;
    }
    for ( int i = i1; i < i2; i++ ) {
        pmaxn = std::max(pmaxn, x[i]);
    }
    if (rank == 0 ) {
        maxn = pmaxn;
        for (int i=1; i < num; i++ ) {
            MPI_Recv(&pmaxn, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &mpi_status);
            maxn = std::max(maxn, pmaxn);
        }
    }
    else {
        MPI_Send(&pmaxn, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0 ) {
        printf("\nmax(x) = %.2f", maxn);
    }
    MPI_Finalize();
}

