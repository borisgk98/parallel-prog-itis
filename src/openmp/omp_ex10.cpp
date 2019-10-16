//
// Created by boris on 16.10.2019.
//

#include <random>
#include <ctime>
#include <climits>
#include <iostream>
#include <omp.h>

void omp_ex10() {
    omp_set_num_threads(3);
    std::mt19937 gen(time(0));
    const int sx= 6, sy = 8;
    int d[sx][sy];
    for (int i = 0; i < sx; i++) {
        for (int j = 0; j < sy; j++) {
            d[i][j] = gen() % 300;
            printf("%d ", d[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    int min = INT_MAX, max = INT_MIN;
#pragma omp parallel for
    for (int i = 0; i < sx; i++) {
#pragma omp parallel for
        for (int j = 0; j < sy; j++) {
//#pragma omp critical
            min = std::min(min, d[i][j]);
//#pragma omp critical
            max = std::max(max, d[i][j]);
        }
    }
    printf("max = %d, min = %d", max, min);
}
