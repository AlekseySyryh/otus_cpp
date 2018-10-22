#pragma once

#include <iostream>
#include <algorithm>
#include <vector>

template<typename T, size_t len>
class allocationBlock {
public:
    allocationBlock() : useMap(len), full() {
        empty = true;
        block = reinterpret_cast<T *>(std::malloc(len * sizeof(T)));
    }

    T *allocate() {
        auto it = std::find_if(useMap.begin(), useMap.end(), [](bool a) { return !a; });
        if (it == useMap.end()) {
            throw std::bad_alloc();
        }
        *it = true;
        full = std::all_of(useMap.begin(), useMap.end(), [](bool a) { return a; });
        empty = false;
        return block + (it - useMap.begin());
    }

    void free(T *free) {
        useMap[free - block] = false;
        full = false;
        empty = std::all_of(useMap.begin(), useMap.end(), [](bool a) { return !a; });
    }

    bool have(T *ref) {
        return ref >= block && ref <= block + len;
    }

    ~allocationBlock() {
        std::free(block);
    }

    bool isEmpty() {
        return empty;
    }

    bool isFull() {
        return full;
    }

private:
    T *block;
    bool empty, full;
    std::vector<bool> useMap;
};
