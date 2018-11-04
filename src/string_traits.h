#pragma once

#include <type_traits>
#include <iostream>

/// \brief Базовая реализация метафункции проверки на строку.
/// \details Если частичные реализации не подходят - это не строка.
template<typename T>
struct is_string {
    static constexpr bool value = false;
};

/// \brief Частичная реализация метафункции проверки на строку.
/// \details Если это реализация шаблона std::basic_string - это строка
template<typename U>
struct is_string<std::basic_string<U>> {
    static constexpr bool value = true;
};

/// \brief Частичная реализация метафункции проверки на строку.
/// \details Если это строковая константа - это строка
template<>
struct is_string<const char *> {
    static constexpr bool value = true;
};

/// Частичная реализация функции print для строк
template<typename T>
typename std::enable_if<is_string<T>::value, std::string>::type print(T value) {
    return value;
}
