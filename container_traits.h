#pragma once

#include <list>
#include <vector>
#include <sstream>

template<typename T>
struct is_container {
    static constexpr bool value = false;
};

//Тоже обобщим... Нам нужно что-то у чего есть константый итератор и два параметра...
//tuple сюда не попадет т.к. не имеет итератора (даже если параметров два)
//basic_string - не попадет из-за трех параметров.
//P.S. Что курил такую проверку константного итератора? Как это вообще можно придумать???
template<template<class, class> class Container, class Val, class Alloc>
struct is_container<Container<Val, Alloc>> {
private:
    template<typename C>
    static uint8_t test(typename C::const_iterator *);

    template<typename C>
    static uint16_t test(...);

public:
    static const bool value = sizeof(test<Container<Val, Alloc>>(0)) == sizeof(uint8_t);
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