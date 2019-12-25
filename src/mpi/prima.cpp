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

void prima_init(int loc_mat[], int loc_pred[], int loc_dist[], int loc_known[],
                int my_rank, int loc_n) {
    int loc_v;

    if (my_rank == 0)
        loc_known[0] = 1;
    else
        loc_known[0] = 0;

    for (loc_v = 1; loc_v < loc_n; loc_v++)
        loc_known[loc_v] = 0;

    for (loc_v = 0; loc_v < loc_n; loc_v++) {
        loc_dist[loc_v] = loc_mat[0 * loc_n + loc_v];
        loc_pred[loc_v] = 0;
    }
}


int find_min_dist(int loc_dist[], int loc_known[], int loc_n) {
    int loc_u = -1, loc_v;
    int shortest_dist = INFINITY;

    for (loc_v = 0; loc_v < loc_n; loc_v++) {
        if (!loc_known[loc_v]) {
            if (loc_dist[loc_v] < shortest_dist) {
                shortest_dist = loc_dist[loc_v];
                loc_u = loc_v;
            }
        }
    }
    return loc_u;
}

std::tuple<int, int, int> find_min_vert(int *mat, int *know, int n, int num, int rank) {
    int loc_u = -1, loc_v = 1;
    int shortest_vert = INFINITY;
    int loc_k = n / num;
    int ru[num], rv[num], rm[num];
    int gru[num], grv[num], grm[num];

//    printf("%d %d %d\n", num, rank, n);
//    printf("%d\n", loc_k);
    for (int i = 0; i < loc_k; i++) {
        int v = i + loc_k * rank;
//        printf("%d\n", v);
        if (!know[v]) {
            for (int u = 0; u < n; u++) {
                if (know[u] && mat[u + i * n] < shortest_vert) {
                    shortest_vert = mat[u + i * n];
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

void prima_compute(int loc_mat[], int loc_dist[], int loc_pred[], int loc_n, int n,
                   MPI_Comm comm) {

    int i, loc_v, loc_u, glbl_u, new_dist, my_rank, dist_glbl_u;
    int *loc_known;
    int my_min[2];
    int glbl_min[2];

    MPI_Comm_rank(comm, &my_rank);
    loc_known = static_cast<int *>(malloc(loc_n * sizeof(int)));

    prima_init(loc_mat, loc_pred, loc_dist, loc_known, my_rank, loc_n);

    /* Run loop n - 1 times since we already know the shortest path to global
       vertex 0 from global vertex 0 */
    for (i = 0; i < n - 1; i++) {
        loc_u = find_min_dist(loc_dist, loc_known, loc_n);

        if (loc_u != -1) {
            my_min[0] = loc_dist[loc_u];
            my_min[1] = loc_u + my_rank * loc_n;
        }
        else {
            my_min[0] = INFINITY;
            my_min[1] = -1;
        }

        /* Get the minimum distance found by the processes and store that
           distance and the global vertex in glbl_min
        */
        MPI_Allreduce(my_min, glbl_min, 1, MPI_2INT, MPI_MINLOC, comm);

        dist_glbl_u = glbl_min[0];
        glbl_u = glbl_min[1];

        /* This test is to assure that loc_known is not accessed with -1 */
        if (glbl_u == -1)
            break;

        /* Check if global u belongs to process, and if so update loc_known */
        if ((glbl_u / loc_n) == my_rank) {
            loc_u = glbl_u % loc_n;
            loc_known[loc_u] = 1;
        }

        /* For each local vertex (global vertex = loc_v + my_rank * loc_n)
           Update the distances from source vertex (0) to loc_v. If vertex
           is unmarked check if the distance from source to the global u + the
           distance from global u to local v is smaller than the distance
           from the source to local v
         */
        for (loc_v = 0; loc_v < loc_n; loc_v++) {
            if (!loc_known[loc_v]) {
                new_dist = dist_glbl_u + loc_mat[glbl_u * loc_n + loc_v];
                if (new_dist < loc_dist[loc_v]) {
                    loc_dist[loc_v] = new_dist;
                    loc_pred[loc_v] = glbl_u;
                }
            }
        }
    }
    free(loc_known);
}

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

    MPI_Barrier(comm);
    for (int j = 0; j < p; j++) {
        if (my_rank == j) {
            Print_matrix(loc_mat, n, n);
        }
        MPI_Barrier(comm);
    }
    prima_compute(loc_mat, loc_dist, loc_pred, loc_n, n, comm);;

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
