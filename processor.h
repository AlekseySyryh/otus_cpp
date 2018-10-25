#include <utility>

#pragma once

#include "ipaddress.h"
#include <vector>
#include <functional>

struct processor {
    explicit processor(
            std::function<void(std::string)> processor) :
            func(std::move(processor)) {}

    void sortVector(std::vector<ipAddress> &ip_pool) {
        sort(ip_pool.begin(), ip_pool.end(), std::greater<>());
    }

    void showAll(std::vector<ipAddress> &ip_pool) const {
        for (const auto &ip : ip_pool) {
            func(ip.getString());
        }
    }

    template<typename ...Args>
    void filter(std::vector<ipAddress> &ip_pool, Args... args) const {
        for (const auto &ip : ip_pool) {
            if (ip.isMatch(args...)) {
                func(ip.getString());
            }
        }
    }


    void filterAny(std::vector<ipAddress> &ip_pool, int value) const {
        for (const auto &ip : ip_pool) {
            if (ip.isMatchAny(value)) {
                func(ip.getString());
            }
        }
    }

private:
    const std::function<void(std::string)> func;
};
