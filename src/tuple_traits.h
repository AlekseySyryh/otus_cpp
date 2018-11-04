#pragma once

#include <type_traits>
#include <sstream>
#include <tuple>
#include <iostream>
#include "all_same_traits.h"

/// \brief Базовая реализация метафункции проверки на кортеж с общим типом
/// \details Если частичные реализации не подходят - это не кортеж с общим типом.
template<typename T>
struct is_sametype_tuple {
    static constexpr bool value = false;
};
/// \brief Частичная реализация метафункции проверки на кортеж с общим типом
/// \details Если это кортеж - проверям что все его типы равны.
template<typename ...Args>
struct is_sametype_tuple<std::tuple<Args...>> {
    static constexpr bool value = all_types_are_same<Args...>::value;
};

/// \brief Базовая реализация метафункции печати i-го элемента кортежа
/// \details Печатаем i-й элемент и вызываем функцию для печати следующего элемента
template<typename T, size_t max = std::tuple_size<T>::value, size_t ix = 0>
struct print_tuple {
    static void print(std::ostringstream &ss, bool first, const T &tuple) {
        if (!first) {
            ss << ".";
        }
        ss << std::get<ix>(tuple);
        print_tuple<T, max, ix + 1>::print(ss, false, tuple);
    }
};

/// \brief Частичная реализация метафункции печати i-го элемента кортежа
/// \details Если мы пытаемся распечатать элемент после последнего - не делаем ничего
template<typename T, size_t ix>
struct print_tuple<T, ix, ix> {
    static void print(__attribute__((unused)) std::ostringstream &ss,
                      __attribute__((unused)) bool first,
                      __attribute__((unused)) const T &tuple) {
    }
};

/// Частичная реализация функции print для кортежей с общим типом
template<typename T>
typename std::enable_if<is_sametype_tuple<T>::value, std::string>::type print(T value) {
    std::ostringstream ss;
    print_tuple<T>::print(ss, true, value);
    return ss.str();
}