#include <iostream>
#include "node.h"
#include <codecvt>
#include <locale>
#include "node.h"
#include <memory>
#include <algorithm>
#include "radix_tree.h"

int main() {
    RadixTree rt;
    std::string line;
    while (std::getline(std::cin, line)) {
        rt.add(line);
    }
    for (auto &row : rt.enumerate()) {
        std::cout << row.first << " " << row.second << std::endl;
    }
    rt.print();
    return 0;
}