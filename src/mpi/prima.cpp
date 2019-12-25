//
// Created by boris on 25.12.2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <utility>
#include <tuple>
#include <algorithm>
#include <iostream>
#include "mpie.h"

#define INFINITY 1000000

void prima_init(int *loc_mat, int *loc_pred, int *loc_known,
                int my_rank, int loc_n);
void prima_compute(int *mat, int *pred, int loc_n, int n,
                   MPI_Comm comm);
std::tuple<int, int, int> find_min_vert(int *mat, int *know, int n, int num, int rank);
void print_result(int global_pred[], int n);

void prima() {
    int *loc_mat, *loc_dist, *loc_pred, *global_dist = NULL, *global_pred = NULL;
    int my_rank, p, loc_n, n;
    MPI_Comm comm;
    MPI_Datatype blk_col_mpi_t;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &p);
    n = Read_n(my_rank, comm);
    loc_n = n / p;
    loc_mat = static_cast<int *>(malloc(n * loc_n * sizeof(int)));
    loc_dist = static_cast<int *>(malloc(loc_n * sizeof(int)));
    loc_pred = static_cast<int *>(malloc(loc_n * sizeof(int)));
    blk_col_mpi_t = Build_blk_col_type(n, loc_n);

    if (my_rank == 0) {
        global_dist = static_cast<int *>(malloc(n * sizeof(int)));
        global_pred = static_cast<int *>(malloc(n * sizeof(int)));
    }
    Read_matrix(loc_mat, n, loc_n, blk_col_mpi_t, my_rank, comm);
    prima_compute(loc_mat, loc_pred, loc_n, n, comm);

    /* Gather the results from Dijkstra */
    MPI_Gather(loc_dist, loc_n, MPI_INT, global_dist, loc_n, MPI_INT, 0, comm);
    MPI_Gather(loc_pred, loc_n, MPI_INT, global_pred, loc_n, MPI_INT, 0, comm);

    /* Print results */
    if (my_rank == 0) {
        print_result(global_pred, n);
        free(global_dist);
        free(global_pred);
    }
    free(loc_mat);
    free(loc_pred);
    free(loc_dist);
    MPI_Type_free(&blk_col_mpi_t);
    MPI_Finalize();
}

void prima_init(int *loc_mat, int *loc_pred, int *loc_known,
                int my_rank, int loc_n) {
    int loc_v;

    if (my_rank == 0)
        loc_known[0] = 1;
    else
        loc_known[0] = 0;

    for (loc_v = 1; loc_v < loc_n; loc_v++)
        loc_known[loc_v] = 0;

    for (loc_v = 0; loc_v < loc_n; loc_v++) {
        loc_pred[loc_v] = 0;
    }
}

void prima_compute(int *mat, int *pred, int loc_n, int n,
                   MPI_Comm comm) {

    int i, my_rank, num;
    int *known;

    MPI_Comm_rank(comm, &my_rank);
    MPI_Comm_size(comm, &num);
    known = static_cast<int *>(malloc(loc_n * sizeof(int)));

    prima_init(mat, pred, known, my_rank, loc_n);

    for (i = 0; i < n - 1; i++) {
        // ищем минимальное ребро среди еще не просмотренных вершин
        std::tuple<int, int, int> v = find_min_vert(mat, known, loc_n, num, my_rank);

        int x = std::get<0>(v), y = std::get<1>(v), s = std::get<2>(v);
        std::printf("%d %d %d\n", x, y, s);
        if (s == INFINITY) {
            break;
        }
        pred[x] = y;
        known[x] = 1;
    }
    free(known);
}

std::tuple<int, int, int> find_min_vert(int *mat, int *know, int n, int num, int rank) {
    int loc_u = -1, loc_v = 1;
    int shortest_vert = INFINITY;
    int loc_k = n / num;
    int ru[num], rv[num], rm[num];
    int gru[num], grv[num], grm[num];

    for (int v = loc_k * rank; v < loc_k * (rank + 1); v++) {
        if (!know[v]) {
            for (int u = 0; u < n; u++) {
                if (know[u] && mat[u * n + v] < shortest_vert) {
                    shortest_vert = mat[u * n + v];
                    loc_u = u;
                    loc_v = v;
                    printf("%d %d %d", v, u, shortest_vert);
                }
            }
        }
    }
    ru[rank] = loc_u;
    rv[rank] = loc_v;
    rm[rank] = shortest_vert;
    MPI_Gather(ru, 1, MPI_INT, gru, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(rv, 1, MPI_INT, grv, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(rm, 1, MPI_INT, grm, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int gu, gv, m = INFINITY;
    for (int i = 0; i < num; i++) {
        if (grm[i] < m) {
            m = grm[i];
            gu = ru[i];
            gv = rv[i];
        }
    }
    return std::make_tuple(gv, gu, m);
}

void print_result(int global_pred[], int n) {
    int v, w, *path, count, i;

    path = static_cast<int *>(malloc(n * sizeof(int)));

    printf("  Result tree\n");
    printf("----    ---------\n");
    for (v = 1; v < n; v++) {
        printf("%d - %d\n", v, global_pred[v]);
    }

    free(path);
}

