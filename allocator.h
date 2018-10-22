#pragma once

#include "allocation_block.h"
#include <list>

template<typename T, int len>
struct myAllocator {
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    std::list<allocationBlock<T, len>> blocks;


    template<typename U>
    struct rebind {
        using other = myAllocator<U, len>;
    };

    T *allocate(std::size_t n) {

        auto it = std::find_if(blocks.begin(), blocks.end(), [](auto &block) {
            return !(block.isFull());
        });

        if (it != blocks.end()) {
            return it->allocate();
        } else {
            blocks.emplace_back();
            return blocks.back().allocate();
        }

    }

    void deallocate(T *p, std::size_t n) {
        auto it = std::find_if(blocks.begin(), blocks.end(), [p](auto &block) {
            return block.have(p);
        });
        if (it != blocks.end()) {
            it->free(p);
            if (it->isEmpty()) {
                blocks.erase(it);
            }
        }
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p) {
        p->~T();
    }
};