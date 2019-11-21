//
// Created by boris on 21.11.19.
//

#include <iostream>
#include <vector>
#include "/opt/mpich2/include/mpi.h"
using namespace std;

void mpi_ex5() {
    int rank, num, n=100;
    double x[n], y[n], res, pres = 0.0;
    MPI_Status Status;

    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(MPI_COMM_WORLD,&num);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if (rank == 0 ) {
        for (int i = 0; i < n; i++) {
            x[i] = i;
            y[i] = n - i;
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
        pres = pres + x[i] * y[i];
    }
    if (rank == 0 ) {
        res = pres;
        for (int i=1; i < num; i++ ) {
            MPI_Recv(&pres, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Status);
            res = res + pres;
        }
    }
    else {
        MPI_Send(&pres, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0 ) {
        printf("\nx (*) y = %.2f", res);
    }
    MPI_Finalize();
}
