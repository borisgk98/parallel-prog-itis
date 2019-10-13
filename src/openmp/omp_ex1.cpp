//
// Created by boris on 13.10.19.
//

#include <omp.h>
#include <cstdio>

void omp_ex1() {
    omp_set_num_threads(8);
#pragma omp parallel
    {
#pragma omp critical
        {
            printf("%s", "Hello World!");
            printf("Task executed with thread %d, Number of threads: %d\n", omp_get_thread_num(), omp_get_num_threads());
        }
    }
}
