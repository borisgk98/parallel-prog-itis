//
// Created by boris on 13.10.19.
//

#include <random>
#include <ctime>
#include <climits>
#include <iostream>
#include <omp.h>

using namespace std;

vector<int> compute_no_parallel(vector<vector<int>> &matrix, vector<int> &v) {
    auto result = vector<int>(v.size());
    for (int i = 0; i < matrix.size(); i++) {
        int sum = 0;
        for (int j = 0; j < matrix[i].size(); j++) {
            sum += v[i] * matrix[i][j];
        }
        result[i] = sum;
    }
    return result;
}

vector<int> compute_parallel(vector<vector<int>> &matrix, vector<int> &v) {
    auto result = vector<int>(v.size());
#pragma omp parallel for schedule(guided,8)
    for (int i = 0; i < matrix.size(); i++) {
        int sum = 0;
#pragma omp parallel for schedule(guided,8)
        for (int j = 0; j < matrix[i].size(); j++) {
            sum += v[i] * matrix[i][j];
        }
        result[i] = sum;
    }
    return result;
}

void print_matrix(vector<vector<int>> &matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (j == 0) {
                printf("||\t");
            }
            printf("%d\t", matrix[i][j]);
            if (j == matrix[i].size() - 1) {
                printf("||\t\n");
            }
        }
    }
}

void print_vector(vector<int> &vector) {
    for (int j = 0; j < vector.size(); j++) {
        printf("||\t%d\t||\n", vector[j]);
    }
}

void print_result(vector<vector<int>> &matrix, vector<int> &vector, std::vector<int> &result) {
    print_matrix(matrix);
    printf("x\n");
    print_vector(vector);
    printf("=\n");
    print_vector(result);
}

void omp_ex9() {
    omp_set_num_threads(8);
    mt19937 gen(time(0));
    bool debug = false;
    int sx = 60, sy = 70;
    auto matrix = vector<vector<int>>(sx, vector<int>(sy));
    for (int i = 0; i < sx; i++) {
        for (int j = 0; j < sy; j++) {
            matrix[i][j] = gen() % 30;
        }
    }
    auto v = vector<int>(sx, 0);
    for (int i = 0; i < sx; i++) {
        v[i] = gen() % 2;
    }

    clock_t begin = clock();
    auto result1 = compute_no_parallel(matrix, v);
    clock_t end = clock();
    if (debug) {
        print_result(matrix, v, result1);
    }
    printf("Total time: %f\n\n", double(end - begin) / CLOCKS_PER_SEC);

    begin = clock();
    auto result2 = compute_parallel(matrix, v);
    end = clock();
    if (debug) {
        print_result(matrix, v, result1);
    }
    printf("Total time: %f\n\n", double(end - begin) / CLOCKS_PER_SEC);
}

