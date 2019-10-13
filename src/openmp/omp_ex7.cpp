//
// Created by boris on 13.10.19.
//

#include <random>
#include <ctime>
#include <climits>
#include <iostream>
#include <omp.h>

void print_array(int *a, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", *(a + i));
    }
    printf("\n");
}

void omp_ex7() {
    std::mt19937 gen(time(0));
    const int size = 10;
    int a[size], b[size], c[size];
    std::fill(a, a + size, -1);
    std::fill(b, b + size, -1);
    std::fill(c, c + size, -1);
    omp_set_num_threads(3);
#pragma omp parallel for schedule(static,2)
    for (int i = 0; i < size; i++) {
        a[i] = gen() % 30;
        b[i] = gen() % 30;
#pragma omp critical
        {
            printf("Thread: %d\n", omp_get_thread_num());
            print_array(a, size);
            print_array(b, size);
        };
    }
    printf("\n");
    omp_set_num_threads(4);
#pragma omp parallel for schedule(dynamic,2)
    for (int i = 0; i < size; i++) {
        c[i] = a[i] + b[i];
#pragma omp critical
        {
            printf("Thread: %d\n", omp_get_thread_num());
            print_array(c, size);
        };
    }
}
