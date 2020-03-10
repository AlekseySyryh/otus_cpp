#pragma once

#include <type_traits>
#include <stack>
#include <sstream>
#include <limits>

/// Частичная реализация функции print для цельночисленных параметров


template<typename T>
typename std::enable_if_t<std::is_integral<T>::value, std::string> print(T value) {
    std::stack<uint8_t> bytes;
    const int divider = std::numeric_limits<uint8_t>::max() + 1;
    for (size_t i = 0; i < sizeof(T); ++i) {
        bytes.emplace(value % divider);
        value /= divider;
    }
    std::ostringstream ss;
    bool first = true;
    while (!bytes.empty()) {
        if (first) {
            first = false;
        } else {
            ss << ".";
        }
        ss << static_cast<int>(bytes.top());
        bytes.pop();
    }
    return ss.str();
}