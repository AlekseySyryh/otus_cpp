#pragma once

#include "split.h"
#include <tuple>
#include <algorithm>
#include <sstream>
#include <range/v3/all.hpp>

class ipAddress {
public:
    explicit ipAddress(const std::string &ip) : ipAddress(splitInt(ip, '.')) {
    }

    ipAddress(std::initializer_list<int> digits) : ipAddress(std::vector<int>(digits)) {
    }

    explicit ipAddress(std::vector<int> digitVector) {
        if (digitVector.size() != digits.size() ||
            ranges::any_of(digitVector, [](int a) { return a < 0 || a > 255; })) {
            throw std::logic_error("Wrong ip");
        }
        ranges::copy(digitVector, digits.data());
    }

    const std::array<int, 4> &getDigits() const {
        return digits;
    }

    std::string getString() const {
        std::ostringstream oss;
        bool first = true;
        ranges::for_each(digits,
                         [&oss, &first](const auto &d) {
                             if (!first) {
                                 oss << ".";
                             } else {
                                 first = false;
                             }
                             oss << d;
                         });
        return oss.str();
    }

#ifdef empty_isMatch_allowed
    bool isMatch() const {
        return true;
    }
#endif

    template<typename ...Args>
    bool isMatch(Args ...args) const {
        static_assert(sizeof...(args) <= digitsInIp, "Too many parameters");
#ifndef empty_isMatch_allowed
        static_assert(sizeof...(args) > 0, "Too few parameters");
#endif
        static_assert(
                std::is_same<std::common_type_t<Args...>, int>::value,
                "Wrong parameter types");
        std::array<int, sizeof...(args)> filter{args...};
        return ranges::equal(filter.begin(), filter.end(),
                             digits.begin(), std::next(digits.begin(), std::min(digits.size(), filter.size())));
    }

    bool isMatchAny(int digit) const {
        return ranges::any_of(digits, [digit](const auto d) { return d == digit; });
    }

private:
    static const size_t digitsInIp = 4;
    std::array<int, digitsInIp> digits;
};

bool operator==(const ipAddress &lhs, const ipAddress &rhs) {
    return lhs.getDigits() == rhs.getDigits();
}

bool operator<(const ipAddress &lhs, const ipAddress &rhs) {
    return lhs.getDigits() < rhs.getDigits();
}

bool operator>(const ipAddress &lhs, const ipAddress &rhs) {
    return lhs.getDigits() > rhs.getDigits();
}