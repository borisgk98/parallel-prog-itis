//
// Created by boris on 16.10.2019.
//
#include <omp.h>
#include <cstdio>
#include <zconf.h>

void out() {
    printf("%s", "Hello World! ");
    printf("Task executed with thread %d, Number of threads: %d\n", omp_get_thread_num(),
           omp_get_num_threads());
}

void s1() {
    int maxt = 8;
#pragma omp parallel shared(maxt)
    {
        while (omp_get_thread_num() != maxt - 1) {
            ;
        }
#pragma omp critical
        {
            out();
            maxt--;
        }
    }
}

void s2() {
#pragma omp parallel for schedule(static, 1)
    for (int i = 0; i < 8; i++) {
        sleep(8 - i);
        out();
    }
}

void s3() {
#pragma omp parallel for schedule(static, 1)
    for (int i = 0; i < 8; i++) {
#pragma omp ordered
        {};
        out();
    }
}

void omp_ex13() {
    omp_set_num_threads(8);
//    s1();
//    s2();
    s3();
}
