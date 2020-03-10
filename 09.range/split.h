#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <range/v3/all.hpp>


std::vector<std::string> split(const std::string &str, char d) {
    std::vector<std::string> r;
    auto start = ranges::begin(str);
    auto stop = ranges::find(str, d);
    auto end = ranges::end(str);
    while (stop != end) {
        r.emplace_back(start, stop);
        start = ranges::next(stop);
        stop = ranges::find(start, end, d);
    }
    r.emplace_back(start, end);

    return r;
}

std::vector<int> splitInt(const std::string &str, char d) {
    auto strings = split(str, d);
    auto ints = std::vector<int>();
    ints.reserve(strings.size());
    ranges::for_each(
            strings,
            [&ints](const std::string &str) {
                return ints.emplace_back(std::stoi(str));
            });
    return ints;
}