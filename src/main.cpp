//#include "openmp/examples/examples.h"
//#include "openmp/openmpe.h"
#include "mpi/mpie.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
//    mpi_ex2();
//	system("sleep 3");

    // cout
    int n = 10;
//    cin >> n;
    int a[n], b[n];
    // rand - -2^31 -- 2^31
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 121 - 20;
        a[i] = *(a + i);
    }
    for (int i = 0; i < n; i++) {
        b[i] = rand() % 121 - 20;
    }

    // res = min |a[i] - b[i]|
    int *res = nullptr;
    for (int i = 0; i < n; i++) {
        if (res == nullptr) {
            int c = abs(a[i] - b[i]);
            res = &c;
        }
        else {
            *res = min(abs(a[i] - b[i]), *res);
        }
    }
    cout << *res << endl;
}