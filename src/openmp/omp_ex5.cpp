//
// Created by boris on 13.10.19.
//

//
// Created by boris on 13.10.19.
//

#include <random>
#include <ctime>
#include <climits>
#include <iostream>
#include <omp.h>

void omp_ex5() {
    omp_set_num_threads(3);
    std::mt19937 gen(time(0));
    const int sx= 6, sy = 8;
    int d[sx][sy];
    for (int i = 0; i < sx; i++) {
        for (int j = 0; j < sy; j++) {
            d[i][j] = gen() % 30;
        }
    }
#pragma omp parallel sections
    {
#pragma omp section
        {
            int sum = 0;
            for (int i = 0; i < sx; i++) {
                for (int j = 0; j < sy; j++) {
                    sum += d[i][j];
                }
            }
#pragma omp critical
            printf("Thread: %d, avg = %d\n", omp_get_thread_num(), sum / (sy * sx));
        }
#pragma omp section
        {
            int min = INT_MAX, max = INT_MIN;
            for (int i = 0; i < sx; i++) {
                for (int j = 0; j < sy; j++) {
                    min = std::min(min, d[i][j]);
                    max = std::max(max, d[i][j]);
                }
            }
#pragma omp critical
            printf("Thread: %d, min = %d, max = %d\n", omp_get_thread_num(), min, max);
        }
#pragma omp section
        {
            int cnt = 0;
            for (int i = 0; i < sx; i++) {
                for (int j = 0; j < sy; j++) {
                    if (d[i][j] % 3 == 0) {
                        cnt++;
                    }
                }
            }
#pragma omp critical
            printf("Thread: %d, cnt = %d\n", omp_get_thread_num(), cnt);
        }
    }
}
