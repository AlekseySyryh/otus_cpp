#pragma once

#include "split.h"
#include <tuple>
#include <algorithm>
#include <sstream>

using ipAddressTuple = std::tuple<int, int, int, int>;

class ipAddress {
public:
    explicit ipAddress(const std::string &ip) : digits() {
        auto digitsStrings = split(ip, '.');
        if (digitsStrings.size() != 4) {
            throw std::logic_error("Wrong ip");
        }
        for (int i = 0; i < 4; ++i) {
            digits[i] = std::stoi(digitsStrings[i]);
            if (digits[i] < 0 || digits[i] > 255) {
                throw std::logic_error("Wrong ip");
            }
        }
        tuple = std::tie(digits[0], digits[1], digits[2], digits[3]);
    }

    ipAddressTuple tuple;

    std::string getAddress() const {
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
    return lhs.tuple == rhs.tuple;
}