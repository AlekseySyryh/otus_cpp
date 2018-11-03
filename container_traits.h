#pragma once

#include <list>
#include <vector>
#include <sstream>

template<typename T>
struct is_container {
    static constexpr bool value = false;
};

//Тоже обобщим... Нам нужно что-то по чему можно итерироваться...
template<template<class ...> class Container, class ... Args>
struct is_container<Container<Args...>> {
    static constexpr bool value = true;

    auto func(Container<Args...> val) {
        for (const auto &t: val) {

        }
    }
};

//...но только не строка.
template<typename U>
struct is_container<std::basic_string<U>> {
    static constexpr bool value = false;
};


template<typename T>
typename std::enable_if<is_container<T>::value, std::string>::type print(T value) {
    std::ostringstream ss;
    bool first = true;
    for (const auto &rec : value) {
        if (first) {
            first = false;
        } else {
            ss << ".";
        }
        ss << rec;
    }
    return ss.str();
}