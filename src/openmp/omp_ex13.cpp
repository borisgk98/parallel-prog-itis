//
// Created by boris on 16.10.2019.
//
#include <omp.h>
#include <cstdio>
#include <zconf.h>

void out() {
    printf("%s", "Hello World! ");
    printf("Task executed with thread %d, Number of threads: %d\n", omp_get_thread_num(),
           omp_get_num_threads());
}

void s1() {
    int maxt = 8;
#pragma omp parallel shared(maxt)
    {
        while (omp_get_thread_num() != maxt - 1) {
            ;
        }
#pragma omp critical
        {
            out();
            maxt--;
        }
    }
}

void s2() {
#pragma omp parallel for schedule(static, 1)
    for (int i = 0; i < 8; i++) {
        sleep(8 - i);
        out();
    }
}

void s3() {
    int myid, i;
#pragma omp parallel private(myid)
    {
        myid = omp_get_thread_num();
#pragma omp for private(i) ordered
        for(i=0; i<8; i++)
            #pragma omp ordered
                printf("T%d: %d\n", 8 - myid, 8 - i);
}
}

void interesting_func(int i) {
    if (i == -1) {
        return;
    }
    #pragma omp parallel
    {
        if (i == omp_get_thread_num()) {
            out();
        }
    }
    interesting_func(i - 1);
}

void s4() {
    interesting_func(7);
}

void s5() {
    int current = 7;
#pragma omp parallel sections
    {
#pragma omp section
        {
            while (omp_get_thread_num() != current);
#pragma omp critical
            {
                out();
                current--;
            };
    }
#pragma omp section
        {
            while (omp_get_thread_num() != current);
#pragma omp critical
            {
                out();
                current--;
            };
        }
#pragma omp section
        {
            while (omp_get_thread_num() != current);
#pragma omp critical
            {
                out();
                current--;
            };
        }
#pragma omp section
        {
            while (omp_get_thread_num() != current);
#pragma omp critical
            {
                out();
                current--;
            };
        }
#pragma omp section
        {
            while (omp_get_thread_num() != current);
#pragma omp critical
            {
                out();
                current--;
            };
        }
#pragma omp section
        {
            while (omp_get_thread_num() != current);
#pragma omp critical
            {
                out();
                current--;
            };
        }
#pragma omp section
        {
            while (omp_get_thread_num() != current);
#pragma omp critical
            {
                out();
                current--;
            };
        }
#pragma omp section
        {
            while (omp_get_thread_num() != current);
#pragma omp critical
            {
                out();
                current--;
            };
        }
    };
}

void omp_ex13() {
    omp_set_num_threads(8);
//    s1();
//    s2();
//    s3();
//    s4();
    s5();
}
