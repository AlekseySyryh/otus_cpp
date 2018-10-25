#pragma once

#include <iostream>
#include <algorithm>
#include <vector>

template<typename T, size_t len>
class allocationBlock {
public:
    allocationBlock() : used(0) {
        block = reinterpret_cast<T *>(std::malloc(len * sizeof(T)));
    }

    T *allocate(size_t req_size) {
        if (getFreeSize() < req_size) {
            throw std::bad_alloc();
        }
        T *retVal = block + used;
        used += req_size;
        return retVal;
    }

    size_t getFreeSize() {
        return len - used;
    }

    ~allocationBlock() {
        std::free(block);
    }


private:
    T *block;
    size_t used;
};
