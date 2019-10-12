#include "omp.h"
#include <iostream>

void increment() {
	int value = 123;
#pragma omp parallel 
	{
#pragma omp atomic
		value++;
#pragma omp critical (cout)
		{
			std::cout << value << std::endl;
		}
	}
}