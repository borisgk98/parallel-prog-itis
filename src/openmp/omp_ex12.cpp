//
// Created by boris on 16.10.2019.
//
#include <random>
#include <ctime>
#include <climits>
#include <iostream>
#include <omp.h>

void omp_ex12() {
    omp_set_num_threads(3);
    std::mt19937 gen(time(0));
    const int n = 30;
    int a[n];
    for (int i = 0; i < n; i++) {
        a[i] = gen() % 300;
        printf("%d ", a[i]);
    }
    printf("\n");
    int max = INT_MIN;
#pragma omp parallel for
    for (int i = 0; i < n; i++) {
        if (a[i] % 7 == 0) {
#pragma omp critical
            max = std::max(max, a[i]);
        }
    }
    if (max != INT_MIN) {
        printf("max = %d", max);
    }
    else {
        printf("Not found");
    }
}
