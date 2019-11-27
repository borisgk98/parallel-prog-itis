#include <iostream>
#include <vector>
#include "/opt/mpich2/include/mpi.h"
using namespace std;

MPI_Status st;


void print(int* n, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", n[i]);
    }
    printf("\n");
}

void printall(int* n, int size) {
    MPI_Barrier(MPI_COMM_WORLD);
    int rank, num;
    MPI_Comm_size(MPI_COMM_WORLD,&num);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    for (int i = 0; i < num; i++) {
        if (rank == i) {
            printf("Proc %d\n", rank);
            print(n, size);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

int scatter(int* sendbuf, int sendcount,
                 int* recvbuf, int recvcount,
                 int root, MPI_Comm comm) {
    int rank, num;
    MPI_Comm_size(comm, &num);
    MPI_Comm_rank(comm, &rank);
    if (root == rank) {
        for (int i = 0; i < num; i++) {
            for (int j = i * recvcount; j < sendcount && j < (i + 1) * recvcount; j++) {
                recvbuf[j % recvcount] = sendbuf[j];
            }
            if (i != root) {
                MPI_Send(recvbuf, recvcount, MPI_INT, i, 0, comm);
            }
        }
        for (int j = root * recvcount; j < sendcount && j < (root + 1) * recvcount; j++) {
            recvbuf[j % recvcount] = sendbuf[j];
        }
    }
    else {
        MPI_Recv(recvbuf, recvcount, MPI_INT, root, 0, comm, &st);
    }
}

int gatter(int* sendbuf, int sendcount,
             int* recvbuf, int recvcount,
             int root, MPI_Comm comm) {
    int rank, num;
    MPI_Comm_size(comm, &num);
    MPI_Comm_rank(comm, &rank);
    if (root == rank) {
        for (int j = rank * sendcount; j < recvcount && j < (rank + 1) * sendcount; j++) {
            recvbuf[j] = sendbuf[j % sendcount];
        }
        for (int i = 0; i < num; i++) {
            if (i != root) {
                int rc = sendcount, recived[rc];
                MPI_Recv(recived, rc, MPI_INT, i, 0, comm, &st);
                for (int j = i * rc; j < recvcount && j < (i + 1) * rc; j++) {
                    recvbuf[j] = recived[j % rc];
                }
            }
        }
    }
    else {
        MPI_Send(sendbuf, sendcount, MPI_INT, root, 0, comm);
    }
}

void mpi_ex8() {
    int rank, num, n=10;
    int x[n];
    for (int i = 0; i < n; i++) {
        x[i] = 0;
    }

    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(MPI_COMM_WORLD,&num);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            x[i] = i;
        }
    }
    int bsz = n / num, buf[bsz];
    scatter(x, n, buf, bsz, 0, MPI_COMM_WORLD);

    for (int i = 0; i < bsz; i++) {
        buf[i] = buf[i] * buf[i];
    }

//    printall(x, n);
//    printall(buf, bsz);
    gatter(buf, bsz, x, n, 0, MPI_COMM_WORLD);
//    printall(buf, bsz);
    if (rank == 0) {
        print(x, n);
    }

    MPI_Finalize();
}
