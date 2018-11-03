#pragma once

#include <type_traits>
#include <sstream>
#include <tuple>
#include <iostream>
#include "all_same_traits.h"

template<typename T>
struct is_tuple {
    static constexpr bool value = false;
};

template<typename ...Args>
struct is_tuple<std::tuple<Args...>> {
    static constexpr bool value = all_types_are_same<Args...>::value;
};


template<typename T, size_t max = std::tuple_size<T>::value, size_t ix = 0>
struct print_tuple {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "InfiniteRecursion"
    static void print(std::ostringstream &ss, bool first, const T &tuple) {
        if (!first) {
            ss << ".";
        }
        ss << std::get<ix>(tuple);
        print_tuple<T, max, ix + 1>::print(ss, false, tuple);
#pragma clang diagnostic pop

    }
};

template<typename T, size_t ix>
struct print_tuple<T, ix, ix> {
    static void print(std::ostringstream &ss, bool first, const T &tuple) {
    }
};

template<typename T>
typename std::enable_if<is_tuple<T>::value, std::string>::type print(T value) {
    std::ostringstream ss;
    print_tuple<T>::print(ss, true, value);
    return ss.str();
}