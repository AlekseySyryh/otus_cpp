#include <iostream>
#include "matrix.h"
/// Точка входа в программу
int main() {
    Matrix<int, -1, 1> matrix;
    matrix.set(1, 0);
    std::cout << (matrix[1] == 0) << std::endl;
    return 0;
}