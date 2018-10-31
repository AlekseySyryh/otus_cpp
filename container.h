#pragma once

#include <memory>
#include <algorithm>
#include "container_element.h"
#include "container_iterator.h"

template<class T, class Allocator = std::allocator<container_element<T>>>
class container {
public:
    container(const Allocator &alloc = Allocator()) : allocator(alloc) {
        first = nullptr;
    }

    template<class InputIt>
    container(InputIt firstIt, InputIt lastIt,
              const Allocator &alloc = Allocator()) : container(alloc) {

        std::for_each(firstIt, lastIt, [this](const T &arg) {
            emplace(arg);
        });
    }

    explicit container(const container<T> &other, const Allocator &alloc = Allocator()) :
            container(alloc) {
        container_element<T> *current = nullptr;

        std::for_each(other.begin(), other.end(),
                      [&current, this](const auto &other) {
                          current = emplace_after(current, other.element);
                      });
    }

    container(container<T, Allocator> &&other) :
            container() {

        std::swap(this->first, other.first);
    }

    ~container() {
        while (first != nullptr) {
            container_element<T> *old_first = first;
            first = old_first->next;
            allocator.destroy(old_first);
            allocator.deallocate(old_first, 1);
        }
    }

    void add(const T &value) {
        container_element<T> *old_first = first;
        first = allocator.allocate(1);
        allocator.construct(first, value, old_first);
    }


    template<typename ...Args>
    void emplace(Args &&...args) {
        container_element<T> *old_first = first;
        first = allocator.allocate(1);
        allocator.construct(first, std::forward<Args>(args)..., old_first);
    }

    container_iterator<container_element<T>> begin() const {
        return container_iterator<container_element<T>>(first);
    }

    container_iterator<container_element<T>> end() const {
        return container_iterator<container_element<T>>(nullptr);
    }

private:
    Allocator allocator;
    container_element<T> *first;


    template<typename ...Args>
    container_element<T> *emplace_after(container_element<T> *after, Args &&...args) {
        if (after == nullptr) {
            emplace(args...);
            return first;
        }
        container_element<T> *new_element = allocator.allocate(1);
        allocator.construct(new_element, std::forward<Args>(args)..., new_element->next);
        after->next = new_element;
        return new_element;
    }
};
