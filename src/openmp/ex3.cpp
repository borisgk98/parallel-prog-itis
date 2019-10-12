#include "omp.h"
#include <iostream>
#include <cstdlib>

double compute_pi() {
	omp_set_num_threads(8);
	long in = 0, total = 0;
	const long number_of_iter = 10e7;
#pragma omp  parallel
	{
#pragma omp for
		for (int i = 0; i < number_of_iter; i++) {
			long x, y;
			x = rand();
			y = rand();
            if (x * x + y * y <= (long) RAND_MAX * RAND_MAX) {
#pragma omp atomic
				in++;
			}
#pragma omp atomic
			total++;
			if (i % 50000 == 0) {
#pragma omp critical 
				{
					printf("Current thread: %d; Current PI %f\n", omp_get_thread_num(), (double) in / total * 4);
				}
			}
		}
#pragma omp barrier
	}
	return (double) in / number_of_iter * 4;
}