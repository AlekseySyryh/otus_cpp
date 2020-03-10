#pragma once

#include <utility>
#include "ipaddress.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <range/v3/all.hpp>

struct processor {
    explicit processor(
            std::function<void(std::string)> processor) :
            func(std::move(processor)) {}

    void sortVector(std::vector<ipAddress> &ip_pool) {
        ranges::action::sort(ip_pool, std::greater<>());
    }

    void showAll(const std::vector<ipAddress> &ip_pool) const {
        ranges::for_each(ip_pool, [this](const auto &ip) { func(ip.getString()); });
    }

    template<typename ...Args>
    void filter(std::vector<ipAddress> &ip_pool, Args... args) const {
        ranges::for_each(ip_pool, [this, args...](const auto &ip) {
            if (ip.isMatch(args...)) {
                func(ip.getString());
            }
        });
    }


    void filterAny(std::vector<ipAddress> &ip_pool, int value) const {
        ranges::for_each(ip_pool, [this, value](const auto &ip) {
            if (ip.isMatchAny(value)) {
                func(ip.getString());
            }
        });
    }

private:
    const std::function<void(std::string)> func;
};
