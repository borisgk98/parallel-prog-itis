//
// Created by boris on 21.11.19.
//
//
// Created by boris on 09.11.2019.
//

#include <iostream>
#include <vector>
#include "/opt/mpich2/include/mpi.h"
using namespace std;

void mpi_ex6() {
    int rank, num, n=100, m=50;
    double x[n][m], maxn = 0.0, pmaxn = 0.0, minn = 100500.0, pminn = 100500.0;
    MPI_Status mpi_status;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            x[i][j] = i + j;
        }
    }
        
    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(MPI_COMM_WORLD,&num);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int k = n / num;
    int i1 = k * rank;
    int i2 = k * (rank + 1 );
    if (rank == num - 1 ) {
        i2 = n;
    }
    for ( int i = i1; i < i2; i++ ) {
        for (int j = 0; j < m; j++) {
            pmaxn = std::max(pmaxn, x[i][j]);
            pminn = std::min(pminn, x[i][j]);
        }
    }
    if (rank == 0 ) {
        maxn = pmaxn;
        minn = pminn;
        for (int i=1; i < num; i++ ) {
            MPI_Recv(&pmaxn, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &mpi_status);
            maxn = std::max(maxn, pmaxn);
            minn = std::max(minn, pminn);
        }
    }
    else {
        MPI_Send(&pmaxn, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0 ) {
        printf("\nmax(x) = %.2f\nmin(x) = %.2f\n", maxn, minn);
    }
    MPI_Finalize();
}
