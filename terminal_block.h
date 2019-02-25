#pragma once

#include "block.h"

class TerminalBlock : public Block {
public:
    bool isTerminalBlock() const override {
        return true;
    }

    const std::vector<std::string> &getCommands() const override {
        throw std::logic_error("");
    }

    time_t getTime() const override {
        return 0;
    }

    size_t getNumberOfCommands() const override {
        return 0;
    }

protected:
    void processSpecialCommand(std::string &) override {

    }

    void processRegularCommand(std::string &) override {

    }
};