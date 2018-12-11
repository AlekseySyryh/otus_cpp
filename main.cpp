#include <iostream>
#include <ctime>
#include <limits>

int main(int argc, char *argv[]) {
    std::time_t start_time = std::time(nullptr);
    if (argc != 2) {
        std::cerr << "Следует указать единственный параметр - размер блока" << std::endl;
        return 1;
    }
    int block_size;
    try {
        block_size = std::stoi(argv[1]);
        if (block_size <= 0) {
            throw std::out_of_range("");
        }
    }
    catch (std::logic_error &) {
        std::cerr << "Размер блока должен быть целым, положительным числом, не более "
                  << std::numeric_limits<int>::max() << std::endl;
        return 1;
    }


    std::cout << start_time << " " << block_size << std::endl;
    return 0;
}