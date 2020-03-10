#pragma once

#include <iterator>

template<typename T>
struct container_iterator {
    container_iterator<T>(T *ref) : ref(ref) {}

    T *ref;
};

template<typename T>
bool operator!=(const container_iterator<T> &lhs, const container_iterator<T> &rhs) {
    return lhs.ref != rhs.ref;
}

template<typename T>
void operator++(container_iterator<T> &arg) {
    arg.ref = arg.ref->next;
}

template<typename T>
T operator*(container_iterator<T> &arg) {
    return *arg.ref;
}