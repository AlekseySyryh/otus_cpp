#pragma once

#include <map>
#include <sstream>
#include <shared_mutex>
#include <algorithm>

//Убрать если извращения не нужны :)
//Потенциально более быстрые, но несколько спорные решения
#define CONTROVERSIAL_DECISIONS


std::map<int, std::string> a, b;
std::shared_mutex dataMutex;

void stringToUpper(std::string &query) {
    std::for_each(query.begin(), query.end(), [](auto &c) { c = toupper(c); });
}

std::string insert(std::istringstream &is) {
    std::string tblName;
    is >> tblName;
    stringToUpper(tblName);
    if (is.fail()) {
        return "ERR Table not specified";
    }
    if (tblName != "A" && tblName != "B") {
        return "ERR Table unknown";
    }
    int id;
    is >> id;
    if (is.fail()) {
        return "ERR Id not specified";
    }
    std::string name;
    is >> name;
    if (is.fail()) {
        return "ERR Name not specified";
    }

    {
#ifdef CONTROVERSIAL_DECISIONS
        //Cогласно cppreference emplace не инвалидирует итераторы, а значит одновременный
        //INSERT и INTERSECTION/SYMMETRIC_DIFFERENCE должны быть безопасны. Правда для SYMMETRIC_DIFFERENCE
        //не все так просто, но поскольку у нас есть требование к минимизации блокировок - будем там костылить...
        std::shared_lock lock(dataMutex);
#else
        //Ага так оно как-то спокойнее и безопаснее.
        std::unique_lock lock(dataMutex);
#endif
        std::map<int, std::string> *tbl;
        if (tblName == "A") {
            tbl = &a;
        } else {
            tbl = &b;
        }
        //Все что выше O(1). Эта строка - O(LogN) согласно документации.
        //Применение emplace вместо пары count/insert позволит производить поиск 1 раз вместо двух
        if (!(tbl->emplace(id, name).second)) {
            return "ERR duplicate " + std::to_string(id);
        }
    }
    return "OK";
}

std::string truncate(std::istringstream &is) {
    std::string tblName;
    is >> tblName;
    stringToUpper(tblName);
    if (is.fail()) {
        return "ERR Table not specified";
    }
    if (tblName != "A" && tblName != "B") {
        return "ERR Table unknown";
    }
    {
        //А вот тут только unique_lock, т.к. clear инвалидирует все что только можно
        //и это точно ни с чем не совместимо
        std::unique_lock lock(dataMutex);
        std::map<int, std::string> *tbl;
        if (tblName == "A") {
            tbl = &a;
        } else {
            tbl = &b;
        }
        //Все что выше - O(1), это O(n) согласно документации.
        tbl->clear();
    }
    return "OK";
}

#ifndef CONTROVERSIAL_DECISIONS
std::string to_string(const std::vector<std::pair<const int, std::string>> &results) {
    //O(NLogN) (внутренняя лямбда O(LogN) повторяется N раз)
    std::ostringstream os;
    std::for_each(results.begin(), results.end(),
                  [&os](const auto &rec) {
                      int id = rec.first;
                      os << id << ",";
                      if (a.count(id) == 1) { //O(LogN)
                          os << a[id]; //O(LogN)
                      }
                      os << ",";
                      if (b.count(id) == 1) { //O(LogN)
                          os << b[id]; //O(LogN)
                      }
                      os << std::endl;
                  });
    os << "OK";
    return os.str();
}
#endif

std::string intersection() {
    std::shared_lock lock(dataMutex);
#ifdef CONTROVERSIAL_DECISIONS
    auto ait = a.begin();
    auto bit = b.begin();
    std::ostringstream os;
    //Сложность O(A+B), где A количество элементов в таблице A, B - в таблице B
    //Доказательство: сложность кода внутри цикла - O(1)
    //На каждом шаге цикла мы двигаем как минимум один итератор на шаг вперед
    //Всего таких шагов можно сделать A+B, следовательно цикл выполнится не более чем A+B раз
    while (ait != a.end() && bit != b.end()) { //Если хоть одна из таблиц закончилась - кина больше не будет
        if (ait->first > bit->first) {
            ++bit;
        } else if (ait->first < bit->first) {
            ++ait;
        } else {// ait->first == bit->first
            os << ait->first << "," << ait->second << "," << bit->second << std::endl;
            ++ait;
            ++bit;
        }
    }
    os << "OK";
    return os.str();
#else
    std::vector<std::pair<const int, std::string>> results;
    //Сложность O(A+B), где A количество элементов в таблице A, B - в таблице B, согласно документации
    std::set_intersection(a.begin(), a.end(),
                          b.begin(), b.end(),
                          std::back_inserter(results),
                          [](const auto &a, const auto &b) {
                              return a.first < b.first;
                          });
    //O(NLogN) и это печально...
    return to_string(results);
#endif
}

std::string symmetric_difference() {
    std::shared_lock lock(dataMutex);
#ifdef CONTROVERSIAL_DECISIONS
    //В чем тут проблема с "обычным" кодом? Может быть состояние гонки. Пусть у нас был
    //INSERT A 0 a0
    //А затем одновременно INSERT B 0 b0 и SYMMETRIC_DIFFERENCE.
    //Тогда возможно что std::set_symmetric_difference найдет id 0 которого еще нет в B,
    //но до момента to_string пройдет INSERT
    //И в итоге на выходе SYMMETRIC_DIFFERENCE будет 0,a0,b0 что ну совсем не айс...
    //Будем костылить велосипед
    auto ait = a.begin();
    auto bit = b.begin();
    std::ostringstream os;
    //Сложность O(A+B), где A количество элементов в таблице A, B - в таблице B
    //Доказательство: сложность кода внутри цикла - O(1)
    //На каждом шаге цикла мы двигаем как минимум один итератор на шаг вперед
    //Всего таких шагов можно сделать A+B, следовательно цикл выполнится не более чем A+B раз
    while (ait != a.end() || bit != b.end()) { //Пока хоть одна из таблиц не закончилась...
        if (ait == a.end() || (bit != b.end() && ait->first > bit->first)) {
            os << bit->first << ",," << bit->second << std::endl;
            ++bit;
        } else if (bit == b.end() || ait->first < bit->first) {
            os << ait->first << "," << ait->second << "," << std::endl;
            ++ait;
        } else {// ait->first == bit->first
            ++ait;
            ++bit;
        }
    }
    os << "OK";
    return os.str();
#else
    std::vector<std::pair<const int, std::string>> results;
    //Сложность O(A+B), где A количество элементов в таблице A, B - в таблице B, согласно документации
    std::set_symmetric_difference(a.begin(), a.end(),
                                  b.begin(), b.end(),
                                  std::back_inserter(results),
                                  [](const auto &a, const auto &b) {
                                      return a.first < b.first;
                                  });
    //O(NLogN) и это печально...
    return to_string(results);
#endif
}


std::string query(const std::string &query) {
    std::istringstream is(query);
    std::string cmd;
    is >> cmd;
    stringToUpper(cmd);
    std::string resp;
    if (cmd == "INSERT") {
        return insert(is);
    }
    if (cmd == "TRUNCATE") {
        return truncate(is);
    }
    if (cmd == "INTERSECTION") {
        return intersection();
    }
    if (cmd == "SYMMETRIC_DIFFERENCE") {
        return symmetric_difference();
    }
    return "ERR Unknown command";
}