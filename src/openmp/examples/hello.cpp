#include <iostream>
#include <omp.h>

void hello() {
    omp_set_num_threads(8);
    #pragma omp parallel
    {
		#pragma omp critical 
		{
			printf("%s", "Hello World!");
			printf("Task executed with thread %d, Number of threads: %d\n", omp_get_thread_num(), omp_get_num_threads());
		}
    }
}