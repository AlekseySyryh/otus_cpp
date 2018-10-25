#pragma once

#include <memory>
#include "container_element.h"
#include "container_iterator.h"


template<class T, class Allocator = std::allocator<container_element<T>>>
class container {
public:
    container() : allocator() {
        first = nullptr;
    }

    ~container() {
        while (first != nullptr) {
            container_element<T> *old_first = first;
            first = old_first->next;
            allocator.destroy(old_first);
            allocator.deallocate(old_first, 1);
        }
    }

    void add(T &value) {
        container_element<T> *old_first = first;
        first = allocator.allocate(1);
        allocator.construct(first, value, old_first);
    }

    container_iterator<container_element<T>> begin() {
        return container_iterator<container_element<T>>(first);
    }

    container_iterator<container_element<T>> end() {
        return container_iterator<container_element<T>>(nullptr);
    }

private:
    Allocator allocator;
    container_element<T> *first;
};
