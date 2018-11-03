#include <iostream>
#include <sstream>
#include <forward_list>
#include <cstdint>
#include <limits>
#include <list>
#include <vector>

/*
Далее идет рабочее и эффективное решение. К сожалению:
1. Я не могу проверить его на big-endian архитектуре (возможно что-то надо было бы исправить...)
2. Оно компиляторо-зависимое (вернее требует GCC).
3. И вообще это код слишком сильно отдает Сишным запашком...

Из за этих проблем я просто оставлю его здесь, а ниже приведу менее эффективное, зато точно кросплатформенное решение.


template <typename T>
typename std::enable_if<std::is_integral<T>::value,std::string>::type print(T value) {
    auto *a = reinterpret_cast<uint8_t *>(&value);

#if __BYTE_ORDER == __LITTLE_ENDIAN
    a += sizeof(T)-1;
#endif

    bool first = true;
    std::ostringstream ss;
    for (int i = 0; i < sizeof(T); ++i) {
        if (first) {
            first = false;
        } else {
            ss << ".";
        }
        ss << static_cast<uint>(*a);

#if __BYTE_ORDER == __LITTLE_ENDIAN
        --a;
#elif __BYTE_ORDER == __BIG_ENDIAN
        ++a;
#else
    #error "Unsupported endian"
#endif
    }
    return ss.str();
}
*/

template<typename T>
typename std::enable_if<std::is_integral<T>::value, std::string>::type print(T value) {
    std::forward_list<uint8_t> bytes;
    const int divider = std::numeric_limits<uint8_t>::max() + 1; //Избавляемся от magic number.
    for (int i = 0; i < sizeof(T); ++i) {
        bytes.emplace_front(
                value % divider);//static_cast<uint8_t>(value) - был бы эффективнее но кроссплатформенность...
        value /= divider;//value >>= 8 было бы круче для запутывания врагов, но если рассматривать вместе с предыдущей
        //строкой, то так явно красивее...
    }
    std::ostringstream ss;
    bool first = true;
    while (!bytes.empty()) {
        if (first) {
            first = false;
        } else {
            ss << ".";
        }
        ss << static_cast<int>(bytes.front());
        bytes.pop_front();
    }
    return ss.str();
}

template<typename T>
std::basic_string<T> print(std::basic_string<T> value) {
    return value;
}

int main() {
    std::cout << print(char(-1)) << std::endl;
    std::cout << print(short(0)) << std::endl;
    std::cout << print(int(2130706433)) << std::endl;
    std::cout << print(long(8875824491850138409)) << std::endl;
    std::cout << print(std::string("127.0.0.2")) << std::endl;

    return 0;
}