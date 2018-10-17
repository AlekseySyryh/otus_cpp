#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include "ipaddress.h"
#include "process.h"


int main() {
    try {
        std::vector<ipAddress> ip_pool;

        for (std::string line; std::getline(std::cin, line);) {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.emplace_back(v.at(0));
        }
        //Вынесено в отдельную функцию и декомпозировано для удобства тестирования
        process(ip_pool,
                [](const std::string &result) {
                    std::cout << result << "\n";//Не std::endl - без сбороса буфера будет быстрее
                });
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
