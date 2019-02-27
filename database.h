#pragma once

#include <map>
#include <sstream>
#include <shared_mutex>
#include <algorithm>

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
        std::unique_lock lock(dataMutex);
        std::map<int, std::string> *tbl;
        if (tblName == "A") {
            tbl = &a;
        } else {
            tbl = &b;
        }
        if (tbl->count(id) == 1) {
            return "ERR duplicate " + std::to_string(id);
        }
        (*tbl)[id] = name;
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
        std::unique_lock lock(dataMutex);
        std::map<int, std::string> *tbl;
        if (tblName == "A") {
            tbl = &a;
        } else {
            tbl = &b;
        }
        tbl->clear();
    }
    return "OK";
}

std::string to_string(const std::vector<std::pair<const int, std::string>> &results) {
    std::ostringstream os;
    std::for_each(results.begin(), results.end(),
                  [&os](const auto &rec) {
                      int id = rec.first;
                      os << id << ",";
                      if (a.count(id) == 1) {
                          os << a[id];
                      }
                      os << ",";
                      if (b.count(id) == 1) {
                          os << b[id];
                      }
                      os << std::endl;
                  });
    os << "OK";
    return os.str();
}

std::string intersection() {
    std::shared_lock lock(dataMutex);
    std::vector<std::pair<const int, std::string>> results;
    std::set_intersection(a.begin(), a.end(),
                          b.begin(), b.end(),
                          std::back_inserter(results),
                          [](const auto &a, const auto &b) {
                              return a.first < b.first;
                          });
    return to_string(results);
}

std::string symmetric_difference() {
    std::shared_lock lock(dataMutex);
    std::vector<std::pair<const int, std::string>> results;
    std::set_symmetric_difference(a.begin(), a.end(),
                                  b.begin(), b.end(),
                                  std::back_inserter(results),
                                  [](const auto &a, const auto &b) {
                                      return a.first < b.first;
                                  });
    return to_string(results);
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