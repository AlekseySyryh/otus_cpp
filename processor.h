#pragma once

#include <utility>
#include "ipaddress.h"
#include <vector>
#include <functional>
#include <algorithm>

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
        /* Вообще тут было
           for (const auto &ip : ip_pool) {
            if (ip.isMatch(args...)) {
                func(ip.getString());
            }
        }
        Но после вчерашней лекции я понял как был не прав - ведь линейный поиск уже есть в
        стандартной библиотеке, а я коварно навелосипедил...

         Исправляюсь:*/
        auto it = ip_pool.begin();
        while ((it = std::find_if(
                it,
                ip_pool.end(),
                [args...](const auto &ip) {
                    return ip.isMatch(args...);
                })) != ip_pool.end()) {
            func(it->getString());
            ++it;
        }
        //P.S. Извините не удержался... :)
    }


    void filterAny(std::vector<ipAddress> &ip_pool, int value) const {
        /*for (const auto &ip : ip_pool) {
            if (ip.isMatchAny(value)) {
                func(ip.getString());
            }
        }*/
        auto it = ip_pool.begin();
        while ((it = std::find_if(
                it,
                ip_pool.end(),
                [value](const auto &ip) {
                    return ip.isMatchAny(value);
                })) != ip_pool.end()) {
            func(it->getString());
            ++it;
        }
    }

private:
    const std::function<void(std::string)> func;
};
