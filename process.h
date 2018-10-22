#pragma once

#include "ipaddress.h"
#include <vector>



void sortVector(std::vector<ipAddress> &ip_pool) {
    //O(NLogN)
    sort(ip_pool.rbegin(), ip_pool.rend());
}

template<typename FuncType>
void showAll(const std::vector<ipAddress> &ip_pool, FuncType processor) {//O(N)
    for (const auto &ip : ip_pool) {
        processor(ip.getString());
    }
}

template<typename FuncType>
void filter(std::vector<ipAddress> &ip_pool, int a, FuncType processor) {
    filter(ip_pool, a, -1, processor);
}


template<typename FuncType>
void filter(std::vector<ipAddress> &ip_pool, int a, int b, FuncType processor) {//O(LogN)
//Хочу уточнить, что лично я бы так делать не стал, а ограничился бы банальным и понятным линейным
//поиском с его O(N), но в задании есть критерий "скорость" и нет критерия "понятность", так что сами виноваты :)

    auto it = lower_bound(ip_pool.begin(), ip_pool.end(),
                          ipAddress{a, b == -1 ? 255 : b, 255, 255},
                          [](const auto &lhs, const auto &rhs) {
                              return *lhs.getDigits() > *rhs.getDigits();
                          });

    while (it != ip_pool.end() && it->isMatch(a, b)) {
        processor(it->getString());
        ++it;
    }
}

template<typename FuncType>
void filterAny(const std::vector<ipAddress> &ip_pool, int value, FuncType processor) {//O(N)
    for (const auto &ip : ip_pool) {
        if (ip.isMatchAny(value)) {
            processor(ip.getString());
        }
    }
}

