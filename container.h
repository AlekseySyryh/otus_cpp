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
    container(InputIt first, InputIt last,
              const Allocator &alloc = Allocator()):container(alloc) {

        std::for_each(first, last, [this](const T &arg) {
            emplace(arg);
        });
    }

    template<typename Alloc = std::allocator<container_element<T>>>
    explicit container(const container<T, Alloc> &other, const Allocator &alloc = Allocator()) :
            container(alloc) {
        container_element<T> *current = nullptr;

        std::for_each(other.begin(), other.end(),
                      [&current, this](const auto &other) {
                          if (first == nullptr) {
                              emplace(other.element);
                              current = first;
                          } else {
                              emplace_after(current, other.element);
                              current = current->next;
                          }
                      });
    }

    container(container<T, Allocator> &&other) :
            container(allocator) {

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
    void emplace_after(container_element<T> *after, Args &&...args) {
        container_element<T> *new_element = allocator.allocate(1);
        allocator.construct(new_element, std::forward<Args>(args)..., new_element->next);
        after->next = new_element;
    }
};
