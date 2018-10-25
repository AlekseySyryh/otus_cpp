#pragma once

#include "allocation_block.h"
#include <forward_list>

template<typename T, int len>
struct myAllocator {
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    std::forward_list<allocationBlock<T, len>> blocks;

    template<typename U>
    struct rebind {
        using other = myAllocator<U, len>;
    };

    T *allocate(std::size_t n) {
        for (auto &block : blocks) {
            if (block.getFreeSize() >= n) {
                return block.allocate(n);
            }
        }
        blocks.emplace_front();
        return blocks.front().allocate(n);
    }

    void deallocate(__attribute__((unused)) T *p, __attribute__((unused)) std::size_t n) {
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    }

    void destroy(T *p) {
        p->~T();
    }
};