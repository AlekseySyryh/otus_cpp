#include <iostream>
#include <map>

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


int main() {
    auto map1 = fillMap<std::map<int, int>>();
    std::cout << std::endl;
    auto map2 = fillMap<std::map<int, int, std::less<>, myAllocator < std::pair<const int, int>, 10>> > ();
    for (auto rec : map2) {
        std::cout << rec.second << std::endl;
    }
    return 0;
}