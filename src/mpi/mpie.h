//
// Created by boris on 16.10.2019.
//

#ifndef PARALLEL_ITIS_2019_MPIE_H
#define PARALLEL_ITIS_2019_MPIE_H

#include <mpi.h>

void mpi_ex1();
void mpi_ex2();
void mpi_ex3();
void mpi_ex4();
void mpi_ex5();
void mpi_ex6();
void mpi_ex7();
void mpi_ex8();

// graph
void djikstra();
void prima();
void Print_matrix(int global_mat[], int rows, int cols);
void Print_dists(int global_dist[], int n);
void Print_paths(int global_pred[], int n);
int Read_n(int my_rank, MPI_Comm comm);
MPI_Datatype Build_blk_col_type(int n, int loc_n);
void Read_matrix(int loc_mat[], int n, int loc_n, MPI_Datatype blk_col_mpi_t,
                 int my_rank, MPI_Comm comm);


#endif //PARALLEL_ITIS_2019_MPIE_H
