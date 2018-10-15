#include <cassert>
#include <iostream>
#include <vector>
#include <array>
#include <tuple>
#include <sstream>
#include <algorithm>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d) {
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos) {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}


using ipAddressTuple = std::tuple<int, int, int, int>;

struct ipAddress {
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

    bool isMatch(int a) const {
        return digits[0] == a;
    }

    bool isMatch(int a, int b) const {
        return digits[0] == a && digits[1] == b;
    }

    bool isMatchAny(int digit) const {
        return std::any_of(digits.begin(), digits.end(), [digit](const auto d) { return d == digit; });
    }

private:
    std::array<int, 4> digits;
};

int main(int argc, char const *argv[]) {
    try {
        std::vector<ipAddress> ip_pool;

        for (std::string line; std::getline(std::cin, line);) {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.emplace_back(v.at(0));
        }

        std::sort(ip_pool.rbegin(), ip_pool.rend(),
                  [](const auto &lhs, const auto &rhs) { return lhs.tuple < rhs.tuple; });

        for (const auto &ip : ip_pool) {
            std::cout << ip.getAddress() << std::endl;
        }
        for (const auto &ip : ip_pool) {
            if (ip.isMatch(1)) {
                std::cout << ip.getAddress() << std::endl;
            }
        }
        for (const auto &ip : ip_pool) {
            if (ip.isMatch(46, 70)) {
                std::cout << ip.getAddress() << std::endl;
            }
        }
        for (const auto &ip : ip_pool) {
            if (ip.isMatchAny(46)) {
                std::cout << ip.getAddress() << std::endl;
            }
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
