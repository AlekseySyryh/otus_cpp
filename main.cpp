#include <iostream>
#include <map>
#include "allocator.h"
#include "container.h"

template<typename Map>
auto fillMap(int elements) {
    Map map;
    if (elements > 0) map[0] = 0;
    if (elements > 1) map[1] = 1;
    for (int i = 2; i < elements; ++i) {
        map[i] = map[i - 1] * i;
    }
    return map;
}

template<typename Container>
auto fillContainer(int elements) {
    Container container;
    for (int i = 0; i < elements; ++i) {
        container.add(i);
    }
    return container;
}

int main() {

    auto map1 = fillMap<std::map<int, int>>(10);
    std::cout << std::endl;
    auto map2 = fillMap<std::map<int, int, std::less<>, myAllocator<std::pair<const int, int>, 10>>>(10);
    for (auto rec : map2) {
        std::cout << rec.first << " " << rec.second << std::endl;
    }
    auto container1 = fillContainer<container<int>>(10);
    std::cout << std::endl;
    auto container2 = fillContainer<container<int, myAllocator<container_element<int>, 10>>>(10);

    for (const auto &rec : container2) {
        std::cout << rec << std::endl;
    }

    return 0;
}