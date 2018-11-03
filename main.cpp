#include <iostream>
#include <list>
#include <vector>

#include "int_traits.h"
#include "string_traits.h"
#include "container_traits.h"


int main() {
    std::cout << print(char(-1)) << std::endl;
    std::cout << print(short(0)) << std::endl;
    std::cout << print(int(2130706433)) << std::endl;
    std::cout << print(long(8875824491850138409)) << std::endl;
    std::cout << print(std::string("127.0.0.2")) << std::endl;
    std::cout << print(std::list<int>{1, 2, 3, 5, 3, 2}) << std::endl;
    std::cout << print(std::vector<int>{6, 3, 1, 35, 6}) << std::endl;
    return 0;
}