#pragma once

#include "ipaddress.h"
#include <vector>

void sortVector(std::vector<ipAddress> &ip_pool);

template<typename FuncType>
void showAll(const std::vector<ipAddress> &ip_pool, FuncType processer);

template<typename FuncType>
void filter1(std::vector<ipAddress> &ip_pool, FuncType processer);

template<typename FuncType>
void filter2(std::vector<ipAddress> &ip_pool, FuncType processer);

template<typename FuncType>
void filterAny(const std::vector<ipAddress> &ip_pool, FuncType processer);

template<typename FuncType>
void process(std::vector<ipAddress> &ip_pool, FuncType processer) {
    sortVector(ip_pool);
    showAll(ip_pool, processer);
    filter1(ip_pool, processer);
    filter2(ip_pool, processer);
    filterAny(ip_pool, processer);
}

void sortVector(std::vector<ipAddress> &ip_pool) {
    //O(NLogN)
    sort(ip_pool.rbegin(), ip_pool.rend(),
         [](const auto &lhs, const auto &rhs) { return lhs.tuple < rhs.tuple; });
}

template<typename FuncType>
void showAll(const std::vector<ipAddress> &ip_pool, FuncType processer) {//O(N)
    for (const auto &ip : ip_pool) {
        processer(ip.getAddress());
    }
}

template<typename FuncType>
void filter1(std::vector<ipAddress> &ip_pool, FuncType processer) {//O(LogN)
//Хочу уточнить, что лично я бы так делать не стал, а ограничился бы банальным и понятным линейным
//поиском с его O(N), но в задании есть критерий "скорость" и нет критерия "понятность", так что сами виноваты :)
    auto it = lower_bound(ip_pool.begin(), ip_pool.end(),
                          ipAddress("1.255.255.255"),
                          [](const auto &lhs, const auto &rhs) {
                              return lhs.tuple > rhs.tuple;
                          });

    while (it != ip_pool.end()) {
        if (!it->isMatch(1)) {
            break;
        }
        processer(it->getAddress());
        ++it;
    }
}

template<typename FuncType>
void filter2(std::vector<ipAddress> &ip_pool, FuncType processer) {//O(LogN)
    auto it = lower_bound(ip_pool.begin(), ip_pool.end(),
                          ipAddress("46.70.255.255"),
                          [](const auto &lhs, const auto &rhs) {
                              return lhs.tuple > rhs.tuple;
                          });

    while (it != ip_pool.end()) {
        if (!it->isMatch(46, 70)) {
            break;
        }
        processer(it->getAddress());
        ++it;
    }
}

template<typename FuncType>
void filterAny(const std::vector<ipAddress> &ip_pool, FuncType processer) {//O(N)
    for (const auto &ip : ip_pool) {
        if (ip.isMatchAny(46)) {
            processer(ip.getAddress());
        }
    }
}

