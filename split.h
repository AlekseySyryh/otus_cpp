#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <range/v3/all.hpp>


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

std::vector<int> splitInt(const std::string &str, char d) {
    auto strings = split(str, d);
    auto ints = std::vector<int>();
    ints.reserve(strings.size());
    std::transform(
            std::begin(strings),
            std::end(strings),
            std::back_inserter(ints),
            [](const std::string &str) {
                return std::stoi(str);
            });
    return ints;
}