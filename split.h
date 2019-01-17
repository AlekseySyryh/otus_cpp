#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <range/v3/all.hpp>


std::vector<std::string> split(const std::string &str, char d) {
    auto spl = ranges::action::split(str, d);

    //Не сказать что это прям нужно, но поведение без этих строк повединие тестов изменяется.
    //Формально тесты менять не хорошо (поведение при переписывании меняться не должно).
    //То что поведение поменялось в тех ситуациях которые в данной программе не возникнут - не важно,
    //а ну как это у нас header-only библиотека внезапно :)
    if (str.size() == 0 || str[str.size()-1]==d){
        ranges::action::push_back(spl, "");
    }

    return spl;
}

std::vector<int> splitInt(const std::string &str, char d) {
    auto spl = ranges::action::split(str, d);
    return ranges::view::transform(spl, [](const auto &str) { return std::stoi(str); });
}