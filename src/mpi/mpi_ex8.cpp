#include <iostream>
#include <vector>
#include "/opt/mpich2/include/mpi.h"
using namespace std;

int scatterv(int* sendbuf, int sendcount,
                 int* recvbuf, int recvcount,
                 int root, MPI_Comm comm) {
    int rank, num;
    MPI_Comm_size(comm, &num);
    MPI_Comm_rank(comm, &rank);
    if (root == rank) {
        for (int i = 0; i < num; i++) {
            for (int j = i * recvcount; j < sendcount && j < i * (recvcount + 1); j++) {
                recvbuf[j % recvcount] = sendbuf[j];
            }
            MPI_Send(recvbuf, recvcount, MPI_INT, i, 0, comm);
        }
    }
    MPI_Barrier(comm);
}

void mpi_ex8() {
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
    MPI_Finalize();
}
