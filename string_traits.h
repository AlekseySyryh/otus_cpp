#pragma once

#include <type_traits>
#include <iostream>

template<typename T>
struct is_string {
    static constexpr bool value = false;
};

//Чуть чуть обобщим задачу... Нам сойдет любая реализация basic_string, не только basic_string<char>
template<typename U>
struct is_string<std::basic_string<U>> {
    static constexpr bool value = true;
};
//... и даже строковая константа сойдет.
template<>
struct is_string<const char *> {
    static constexpr bool value = true;
};

template<typename T>
typename std::enable_if<is_string<T>::value, std::string>::type print(T value) {
    return value;
}
