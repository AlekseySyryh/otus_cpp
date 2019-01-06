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
    rt.add("aleksey");
    rt.add("sasha");
    rt.add("aleks");
    rt.add("alek");
    rt.add("alesha");
    rt.add("maksim");
    for (auto &row : rt.enumerate()) {
        std::cout << row.first << " " << row.second << std::endl;
    }
    rt.print();
    return 0;
}