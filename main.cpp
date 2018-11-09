#include <iostream>
#include "matrix.h"
/// Точка входа в программу
int main() {
    Matrix<int, -1, 3> matrix;
    matrix[0][0][0] = 0;
    matrix[1][1][1] = 1;
    matrix[2][2][2] = 2;
    matrix[3][3][3] = -1;
    for (auto a: matrix) {
        std::cout << std::get<0>(a) << " " << std::get<1>(a) << " " << std::get<2>(a) << " " << std::get<3>(a)
                  << std::endl;
    }
    return 0;
}