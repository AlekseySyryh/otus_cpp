#include <iostream>
#include <iomanip>
#include "matrix.h"


int main() {
    Matrix<int, 0> matrix;
    for (int i = 0; i < 10; ++i) {
        matrix[i][i] = i;
    }
    for (int i = 0; i < 10; ++i) {
        matrix[i][9 - i] = 9 - i;
    }
    std::cout << "Фрагмент от [1,1] до [8,8]" << std::endl;
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << "Занято ячеек: " << matrix.size() << std::endl;
    std::cout << "Занятые ячейки:" << std::endl <<
              std::setw(10) << std::left << "X" <<
              std::setw(10) << std::left << "Y" <<
              "Значение" << std::endl;
    for (const auto cell : matrix) {
        size_t x, y;
        int val;
        std::tie(x, y, val) = cell;
        std::cout << std::setw(10) << std::left << x << std::setw(10) << std::left << y << val << std::endl;
    }
    return 0;
}