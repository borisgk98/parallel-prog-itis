//
// Created by boris on 13.10.19.
//

#include <omp.h>
#include <cstdio>
#include <iostream>

void omp_ex3() {
    int a = 20, b = 30;

    int num_of_t = 2;
    omp_set_num_threads(num_of_t);
    std::cout << a << " " << b << std::endl;
#pragma omp parallel private(a) firstprivate(b)
    {
        a += omp_get_thread_num();
        b += omp_get_thread_num();
#pragma omp critical
        std::cout << a << " " << b << std::endl;
    };
    std::cout << a << " " << b << std::endl;

    num_of_t = 4;
    omp_set_num_threads(num_of_t);
    std::cout << a << " " << b << std::endl;
#pragma omp parallel shared(a) private(b)
    {
        a -= omp_get_thread_num();
        b -= omp_get_thread_num();
#pragma omp critical
        std::cout << a << " " << b << std::endl;
    };
    std::cout << a << " " << b << std::endl;
}
