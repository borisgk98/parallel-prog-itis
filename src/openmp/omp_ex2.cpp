//
// Created by boris on 13.10.19.
//

#include <omp.h>
#include <cstdio>
#include <iostream>

void omp_ex2() {
    int num_of_t = 3;
    omp_set_num_threads(num_of_t);
#pragma omp parallel if (num_of_t > 2)
    {
        printf("Task executed with thread %d, Number of threads: %d\n", omp_get_thread_num(), omp_get_num_threads());
    };
    num_of_t = 2;
    omp_set_num_threads(num_of_t);
#pragma omp parallel if (num_of_t > 2)
    {
        printf("Task executed with thread %d, Number of threads: %d\n", omp_get_thread_num(), omp_get_num_threads());
    };
}