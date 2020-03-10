#pragma once

#include <list>
#include <vector>
#include <sstream>

/// \brief Базовая реализация метафункции проверки на принадлежность контейнеру
/// \details Если не указано иное то это не контейнер
template<typename T>
struct is_container {
    static constexpr bool value = false;
};

/// \brief Частичная реализация метафункции проверки на принадлежность контейнеру, для случая со строками
/// \details Строка (в рамках нашей задачи) точно не контейнер (хотя такая проверка и так есть но...)
template<typename T>
struct is_container<std::basic_string<T>> {
    static constexpr bool value = false;
};

/// \brief Частичная реализация метафункции проверки на принадлежность контейнеру, для случая шаблона с двумя параметрами
/// \details Если у типа есть константный итератор - это контейнер, иначе - нет.

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

/// Частичная реализация функции print для контейнеров одним типом значения и константными итераторами (кроме строк)
template<typename T>
typename std::enable_if_t<is_container<T>::value, std::string> print(T value) {
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