#include <iostream>
#include <vector>
#include "/opt/mpich2/include/mpi.h"

using namespace std;

int ProcRank, ProcNum;

void
ProcessTermination(double *matrix, double *vector, double *pDouble2, double *pDouble3, double *pProcResult, int size);

void RandomDataInitialization(double *&matrix, double *&vector, int &size) {
    for (int i = 0; i < size; i++) {
        vector[i] = i;
    }
    for (int i = 0; i < size*size; i++) {
        matrix[i] = i;
    }
}

void ProcessInitialization(double *&pMatrix, double *&pVector, double *&pResult, double *&pProcRows, double *&pProcResult,
                      int &Size, int &RowNum) {
    int RestRows;
    int i;
    if (ProcRank == 0) {
        do {
            printf("\nEnter size of the initial objects: ");
            scanf("%d", &Size);
            if (Size < ProcNum) {
                printf("Size of the objects must be greater than number of processes! \n ");
            }
        } while (Size < ProcNum);
    }
    MPI_Bcast(&Size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    RestRows = Size;
    for (i = 0; i < ProcRank; i++) RestRows = RestRows - RestRows / (ProcNum - i);
    RowNum = RestRows / (ProcNum - ProcRank);
    pVector = new double[Size];
    pResult = new double[Size];
    pProcRows = new double[RowNum * Size];
    pProcResult = new double[RowNum];
    if (ProcRank == 0) {
        pMatrix = new double[Size * Size];
        RandomDataInitialization(pMatrix, pVector, Size);
    }
}

void ParallelResultCalculation(double *pProcRows, double *pVector, double *pProcResult, int Size, int RowNum) {
    int i, j;
    for (i = 0; i < RowNum; i++) {
        pProcResult[i] = 0;
        for (j = 0; j < Size; j++) pProcResult[i] += pProcRows[i * Size + j] * pVector[j];
    }
}

void DataDistribution(double *pMatrix, double *pProcRows, double *pVector, int Size, int RowNum) {
    int *pSendNum; // the number of elements sent to the process
    int *pSendInd;
    int RestRows = Size;
    MPI_Bcast(pVector, Size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    pSendInd = new int[ProcNum];
    pSendNum = new int[ProcNum];
    RowNum = (Size / ProcNum);
    pSendNum[0] = RowNum * Size;
    pSendInd[0] = 0;
    for (int i = 1; i < ProcNum; i++) {
        RestRows -= RowNum;
        RowNum = RestRows / (ProcNum - i);
        pSendNum[i] = RowNum * Size;
        pSendInd[i] = pSendInd[i - 1] + pSendNum[i - 1];
    }
    MPI_Scatterv(pMatrix, pSendNum, pSendInd, MPI_DOUBLE, pProcRows, pSendNum[ProcRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    delete[] pSendNum;
    delete[] pSendInd;
}

void ResultReplication(double *pProcResult, double *pResult, int Size, int RowNum) {
    int i;
    int *pReceiveNum;
    int *pReceiveInd;
    int RestRows = Size;
    pReceiveNum = new int[ProcNum];
    pReceiveInd = new int[ProcNum];
    pReceiveInd[0] = 0;
    pReceiveNum[0] = Size / ProcNum;
    for (i = 1; i < ProcNum; i++) {
        RestRows -= pReceiveNum[i - 1];
        pReceiveNum[i] = RestRows / (ProcNum - i);
        pReceiveInd[i] = pReceiveInd[i - 1] + pReceiveNum[i - 1];
    }
    MPI_Allgatherv(pProcResult, pReceiveNum[ProcRank], MPI_DOUBLE, pResult, pReceiveNum, pReceiveInd, MPI_DOUBLE,
                   MPI_COMM_WORLD);
    delete[] pReceiveNum;
    delete[] pReceiveInd;
}

void mpi_ex7() {
//    double *pMatrix;// The first argument - initial matrix
//    double *pVector;  // The second argument - initial vector
//    double *pResult;  // Result vector for matrix-vector multiplication
//    int Size;         // Sizes of initial matrix and vector
//    double *pProcRows;
//    double *pProcResult;
//    int RowNum;
//    double Start, Finish, Duration;
//    MPI_Init(nullptr, nullptr);
//    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
//    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
//    ProcessInitialization(pMatrix, pVector, pResult, pProcRows, pProcResult, Size, RowNum);
//    DataDistribution(pMatrix, pProcRows, pVector, Size, RowNum);
//    ParallelResultCalculation(pProcRows, pVector, pProcResult, Size, RowNum);
//    ResultReplication(pProcResult, pResult, Size, RowNum);
//    ProcessTermination(pMatrix, pVector, pResult, pProcRows, pProcResult, Size);
//    MPI_Finalize();

//    int rank, num, n=100, m=50;
//    double x[n][m], ;
//    MPI_Status mpi_status;
//
//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < m; j++) {
//            x[i][j] = i + j;
//        }
//    }
//
//    MPI_Init(nullptr, nullptr);
//    MPI_Comm_size(MPI_COMM_WORLD,&num);
//    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
//
//    int k = n / num;
//    int i1 = k * rank;
//    int i2 = k * (rank + 1 );
//    if (rank == num - 1 ) {
//        i2 = n;
//    }
//    for ( int i = i1; i < i2; i++ ) {
//        for (int j = 0; j < m; j++) {
//
//        }
//    }
//    if (rank == 0 ) {
//        for (int i=1; i < num; i++ ) {
////            MPI_Recv(&pmaxn, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &mpi_status);
//        }
//    }
//    else {
////        MPI_Send(&pmaxn, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
//    }
//    if (rank == 0 ) {
//
//    }
//    MPI_Finalize();

    const int N = 3;
    int rank, size, procN;
    double a[N*N], b[N], c[N];
    double *procA, *procC;

    MPI_Init(nullptr, nullptr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL));
    if (!rank) // rank == 0
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                a[i*N + j] = i * N + j;// (double)(rand() % 100) - 50;
            b[i] = i;// (double)(rand() % 100) - 50;
        }
    }

    int* len = new int[size], *lenc = new int[size];
    int* ind = new int[size], *indc = new int[size];

    int rest = N;
    int k = rest / size;
    len[0] = k * N; lenc[0] = k;
    ind[0] = 0;     indc[0] = 0;
    for (int i = 1; i < size; i++)
    {
        rest -= k;
        k = rest / (size - i);
        len[i] = k * N;    lenc[i] = k;
        ind[i] = ind[i - 1] + len[i - 1];
        indc[i] = indc[i - 1] + lenc[i - 1];
    }

    procN = len[rank];
    procA = new double[procN];
    procC = new double[lenc[rank]];

    MPI_Scatterv(a, len, ind, MPI_DOUBLE, procA, procN, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);
    MPI_Bcast(b, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for(int i=0; i<procN/N; i++)
    {
        procC[i] = 0;
        //	printf("rank=%d ", rank);
        //	printf(" procA[%d,0]=%f ", i, procA[i*N]);
        for (int j = 0; j < N; j++)
            procC[i] += procA[i*N + j] * b[j];
        //printf(" procA[%d,%d]=%f ", i,j, procA[i*N + j]);
        //printf("procC[%d] = %f\n",i,procC[i]);
    }
    //printf("rank=%d len = %d  ind = %d\n", rank,len[rank],ind[rank]);

    MPI_Gatherv(procC, lenc[rank], MPI_DOUBLE, c, lenc, indc, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    if (!rank) // rank == 0
    {
        for (int i = 0; i < N; i++)
        {
            printf("c[%d] = %f\n",i,c[i]);
        }
    }

    MPI_Finalize();
}

void
ProcessTermination(double *matrix, double *vector, double *pResult, double *pDouble3, double *pProcResult, int size) {
    if (ProcRank == 0) {

        cout << "Vector" << endl;
        for (int i = 0; i < size; i++) {
            cout << vector[i] << " ";
        }
        cout << endl;
        cout << "Matrix" << endl;
        for (int i = 0; i < size*size; i++) {
            cout << matrix[i] << " ";
            if (i % size == size - 1) {
                cout << endl;
            }
        }
        cout << endl;
        cout << "Result matrix" << endl;
        for (int i = 0; i < size * size; i++) {
            cout << pResult[i] << " ";
            if (i % size == size - 1) {
                cout << endl;
            }
        }
        cout << endl;
    }
}
