#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

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
    //Тут у нас не большой конфликт. С одной стороны DRY, с другой стороны так чутка по медленее будет.
    //Но думаю это грех не большой...
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