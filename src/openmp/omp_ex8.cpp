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

void for_body(int *a, int *b, int size, int i, bool debug) {
    b[i] = (double)(a[i-1] + a[i] + a[i+1]) / 3;
    if (debug) {
#pragma omp critical
        {
            printf("Thread: %d\n", omp_get_thread_num());
            print_array(b, size);
        };
    }
}

void parallel_static(int *a, int size, int block_size, bool debug) {
    printf("Static schedule start. Block size = %d\n", block_size);
    int b[size];
    std::fill(b, b + size, -1);
    clock_t begin = clock();
#pragma omp parallel for schedule(static, block_size)
    for (int i = 1; i < size - 1; i++) {
        for_body(a, b, size, i, debug);
    }
    clock_t end = clock();
    if (debug) {
        print_array(b, size);
    }
    printf("Total time: %f\n\n\n", double(end - begin) / CLOCKS_PER_SEC);
}

void parallel_dynamic(int *a, int size, int block_size, bool debug) {
    printf("Dynamic schedule start. Block size = %d\n", block_size);
    int b[size];
    std::fill(b, b + size, -1);
    clock_t begin = clock();
#pragma omp parallel for schedule(dynamic, block_size)
    for (int i = 1; i < size - 1; i++) {
        for_body(a, b, size, i, debug);
    }
    clock_t end = clock();
    if (debug) {
        print_array(b, size);
    }
    printf("Total time: %f\n\n\n", double(end - begin) / CLOCKS_PER_SEC);
}

void parallel_guided(int *a, int size, int block_size, bool debug) {
    printf("Guided schedule start. Block size = %d\n", block_size);
    int b[size];
    std::fill(b, b + size, -1);
    clock_t begin = clock();
#pragma omp parallel for schedule(guided, block_size)
    for (int i = 1; i < size - 1; i++) {
        for_body(a, b, size, i, debug);
    }
    clock_t end = clock();
    if (debug) {
        print_array(b, size);
    }
    printf("Total time: %f\n\n\n", double(end - begin) / CLOCKS_PER_SEC);
}

void parallel_runtime(int *a, int size, bool debug) {
    printf("Runtime (auto) schedule start.\n");
    int b[size];
    std::fill(b, b + size, -1);
    clock_t begin = clock();
#pragma omp parallel for schedule(runtime)
    for (int i = 1; i < size - 1; i++) {
        for_body(a, b, size, i, debug);
    }
    clock_t end = clock();
    if (debug) {
        print_array(b, size);
    }
    printf("Total time: %f\n\n\n", double(end - begin) / CLOCKS_PER_SEC);
}

void omp_ex8() {
    omp_set_num_threads(8);
    std::mt19937 gen(time(0));
    const int size = 160000;
    int a[size];
    for (int i = 0; i < size; i++) {
        a[i] = i;
    }
    bool debug = false;
    int block_size = 4;
    parallel_static(a, size, block_size, debug);
    parallel_dynamic(a, size, block_size, debug);
    parallel_guided(a, size, block_size, debug);
    parallel_runtime(a, size, debug);
}
