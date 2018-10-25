#include <iostream>
#include <map>
#include "allocator.h"
#include "container.h"

template<typename Map>
auto fillMap() {
    Map map;
    map[0] = 0;
    map[1] = 1;
    for (int i = 2; i < 10; ++i) {
        map[i] = map[i - 1] * i;
    }
    return map;
}

class a {
public:
    a() {
        std::cout << "CA" << std::endl;
    }

    ~a() {
        std::cout << "DA" << std::endl;
    }
};

int main() {
    auto map1 = fillMap<std::map<int, int>>();
    std::cout << std::endl;
    auto map2 = fillMap<std::map<int, int, std::less<>, myAllocator < std::pair<const int, int>, 10>> > ();
    for (auto rec : map2) {
        std::cout << rec.first << " " << rec.second << std::endl;
    }
    container<int> container1;
    for (int i = 0; i < 10; ++i) {
        container1.add(i);
    }
    container<int, myAllocator<container_element<int>, 10>> container2;
    for (int i = 0; i < 10; ++i) {
        container2.add(i);
    }
    for (const auto &rec : container2) {
        std::cout << rec << std::endl;
    }

    return 0;
}