#pragma once

#include "split.h"
#include <tuple>
#include <algorithm>
#include <sstream>

class ipAddress {
public:
    explicit ipAddress(const std::string &ip) : ipAddress(splitInt(ip, '.')) {
    }

    ipAddress(std::initializer_list<int> digits) : ipAddress(std::vector<int>(digits)) {
    }

    explicit ipAddress(std::vector<int> digitsStrings) : digits() {
        if (digitsStrings.size() != 4) {
            throw std::logic_error("Wrong ip");
        }
        for (int i = 0; i < 4; ++i) {
            digits[i] = digitsStrings[i];
            if (digits[i] < 0 || digits[i] > 255) {
                throw std::logic_error("Wrong ip");
            }
        }
    }

    const std::array<int, 4> *getDigits() const {
        return &digits;
    }

    std::string getString() const {
        std::ostringstream oss;
        bool first = true;
        for (auto d : digits) {
            if (!first) {
                oss << ".";
            } else {
                first = false;
            }
            oss << d;

        }
        return oss.str();
    }

#ifdef empty_isMatch_allowed
    bool isMatch() const {
        return true;
    }
#endif

    template<typename ...Args>
    bool isMatch(Args ...args) const {
        static_assert(sizeof...(args) <= 4, "Too many parameters");
#ifndef empty_isMatch_allowed
        static_assert(sizeof...(args) > 0, "Too few parameters");
#endif
        static_assert(
                std::is_same<std::common_type_t<Args...>, int>::value,
                "Wrong parameter types");
        std::array<int, sizeof...(args)> filter{args...};
        return std::equal(filter.begin(), filter.end(), digits.begin());
    }

    bool isMatchAny(int digit) const {
        return std::any_of(digits.begin(), digits.end(), [digit](const auto d) { return d == digit; });
    }


private:
    std::array<int, 4> digits;
};

bool operator==(const ipAddress &lhs, const ipAddress &rhs) {
    return *lhs.getDigits() == *rhs.getDigits();
}

bool operator<(const ipAddress &lhs, const ipAddress &rhs) {
    return *lhs.getDigits() < *rhs.getDigits();
}

bool operator>(const ipAddress &lhs, const ipAddress &rhs) {
    return *lhs.getDigits() > *rhs.getDigits();
}