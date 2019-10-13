//
// Created by boris on 13.10.19.
//

#include <random>
#include <ctime>
#include <climits>
#include <iostream>
#include <omp.h>

void omp_ex6() {
    std::mt19937 gen(time(0));
    const int size = 100;
    int a[size], strue = 0;
    for (int i = 0; i < size; i++) {
        a[i] = gen() % 30;
        strue += a[i];
    }
    printf("True avg = %f\n", (double) strue / size);

    int sum = 0;
#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < size; i++) {
        sum += a[i];
    }
    printf("%f\n", (double) sum / size);

    sum = 0;
#pragma omp parallel for
    for (int i = 0; i < size; i++) {
        sum += a[i];
    }
    printf("%f\n", (double) sum / size);
}