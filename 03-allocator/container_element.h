#pragma once

#include <ostream>

template<typename T>
struct container_element {
    container_element(const T &el, container_element<T> *next) : element(el), next(next) {}

    T element;
    container_element<T> *next;
};

template<typename T>
std::ostream &operator<<(std::ostream &stream, const container_element<T> &arg) {
    stream << arg.element;
    return stream;
}