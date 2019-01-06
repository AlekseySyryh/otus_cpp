#pragma once

#include <codecvt>
#include <locale>
#include <unordered_map>
#include <memory>
#include <string_view>
#include <algorithm>

class node {

public:
    explicit node(const std::string &str) {
        label = converter.from_bytes(str);
        is_end = true;
    }

    explicit node(const std::wstring &wstr) {
        label = wstr;
        is_end = true;
    }

    const std::wstring &getLabel() const {
        return label;
    }

    std::string getLabelAsString() const {
        return converter.to_bytes(label);
    }

    std::string getFirstLetterAsString() const {
        return converter.to_bytes(label[0]);
    }

    void add(const std::string &str) {
        std::wstring wstr = converter.from_bytes(str);
        add(wstr);
    }

    void add(const std::wstring &wstr) {
        std::wstring::const_iterator labelDiff, wstrDiff;
        std::tie(labelDiff, wstrDiff) = std::mismatch(label.cbegin(), label.cend(), wstr.cbegin(), wstr.cend());

        if (labelDiff != label.end()) {
            auto newNode = std::make_unique<node>(std::wstring(labelDiff, label.cend()));
            for (auto &childRec : childs) {
                newNode->childs[childRec.first] = std::move(childRec.second);
            }
            childs.clear();
            childs[*labelDiff] = std::move(newNode);
            label = std::wstring(label.cbegin(), labelDiff);
            is_end = wstrDiff == wstr.cend();
        }

        if (wstrDiff != wstr.end()) {
            if (!childs[*wstrDiff]) {
                childs[*wstrDiff] = std::make_unique<node>(std::wstring(wstrDiff, wstr.cend()));
            } else {
                childs[*wstrDiff]->add(std::wstring(wstrDiff, wstr.cend()));
            }
        }
    }

    bool getIsEnd() const {
        return is_end;
    }

    const std::unordered_map<wchar_t, std::unique_ptr<node>> &getChilds() const {
        return childs;
    }

private:
    mutable std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring label;
    std::unordered_map<wchar_t, std::unique_ptr<node>> childs;
    bool is_end;
};
