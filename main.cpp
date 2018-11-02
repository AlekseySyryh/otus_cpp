#include <iostream>
#include <sstream>


template <typename T>
typename std::enable_if<std::is_integral<T>::value,std::string>::type print(T value) {
    auto *a = reinterpret_cast<uint8_t *>(&value);

#if __BYTE_ORDER == __LITTLE_ENDIAN
    a += sizeof(T)-1;
#endif

    bool first = true;
    std::stringstream ss;
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

int main() {
    std::cout << print(char(-1)) << std::endl;
    std::cout << print(short(0)) << std::endl;
    std::cout << print(int(2130706433)) << std::endl;
    std::cout << print(long(8875824491850138409)) << std::endl;
    return 0;
}