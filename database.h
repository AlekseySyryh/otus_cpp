#pragma once

#include <map>
#include <sstream>
#include <shared_mutex>

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
    return "ERR Unknown command";
}