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

    bool isMatch(int a, int b = -1) const {
        return digits[0] == a && (b == -1 || digits[1] == b);
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