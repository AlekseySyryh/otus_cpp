#pragma once

#include <vector>
#include "node.h"
#include <iostream>

class RadixTree {
public:
    void add(const std::string &str) {
        if (!root) {
            root = std::make_unique<node>(str);
        } else {
            root->add(str);
        }
    }

    const std::unique_ptr<node> &getRoot() const {
        return root;
    }

    std::vector<std::pair<std::string, std::string>> enumerate() const {
        std::vector<std::pair<std::string, std::string>> res;
        if (root) {
            enumerateNode(*root, "", res);
        }
        return res;
    }

    void print() const {
        if (root) {
            printNode(*root, "", true);
        }
    }
private:
    std::unique_ptr<node> root;

    void printNode(const node &node, const std::string &prefix, bool isLast) const {
        std::cout << prefix << "+ \"" << node.getLabelAsString() << "\"";
        if (node.getIsEnd()) {
            std::cout << "$";
        }
        std::cout << std::endl;
        size_t remain = node.getChilds().size();
        for (auto &child : node.getChilds()) {
            --remain;
            printNode(*child.second, prefix + (isLast ? "  " : "| "), remain == 0);
        }
    }
    void enumerateNode(const node &node, const std::string &prefix,
                       std::vector<std::pair<std::string, std::string>> &result) const {
        if (node.getIsEnd()) {
            result.emplace_back(std::make_pair(
                    prefix + node.getLabelAsString(),
                    prefix + node.getFirstLetterAsString()));
        }
        for (auto &child : node.getChilds()) {
            enumerateNode(*child.second, prefix + node.getLabelAsString(), result);
        }
    }
};