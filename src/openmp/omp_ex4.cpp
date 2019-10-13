//
// Created by boris on 13.10.19.
//

#include <random>
#include <ctime>
#include <climits>
#include <iostream>
#include <omp.h>

void omp_ex4() {
    omp_set_num_threads(2);
    std::mt19937 gen(time(0));
    const int size = 10;
    int a[size], b[size];
    for (int i = 0; i < size; i++) {
        *(a + i) = gen() % 30;
        *(b + i) = gen() % 30;
    }
    int min = INT_MAX, max = INT_MIN;
#pragma omp parallel sections
    {
#pragma omp section
    {
        for (int i = 0; i < size; i++) {
            if (*(a + i) < min) {
                min = *(a + i);
            }
        }
    }
#pragma omp section
    {
        for (int i = 0; i < size; i++) {
            if (*(b + i) > max) {
                max = *(b + i);
            }
        }
    }
}
    std::cout << max << " " << min << std::endl;
}
