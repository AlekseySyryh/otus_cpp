#pragma once

#include <tuple>
#include <functional>
#include <vector>

class Block {
public:
    Block() {
        nextBlockFunc = []() ->
                std::shared_ptr<Block> {
            throw std::logic_error("Тип следующего блока не определен, так как блок еще не закончен.");
        };
    }

    virtual bool isTerminalBlock() const {
        return false;
    }

    void addCommand(std::string &command) {
        if (command == "{" || command == "}") {
            processSpecialCommand(command);
        } else {
            processRegularCommand(command);
        }
    }

    virtual std::time_t getTime() const = 0;

    virtual const std::vector<std::string> &getCommands() const = 0;

    virtual size_t getNumberOfCommands() const = 0;

    virtual void close() {}

    virtual void clear() {}
    bool isComplete() const {
        return complete;
    }

    std::function<std::shared_ptr<Block>()> getNextBlockBuilder() const {
        return nextBlockFunc;
    }

protected:
    std::function<std::shared_ptr<Block>()> nextBlockFunc;

    virtual void processSpecialCommand(std::string &) = 0;

    virtual void processRegularCommand(std::string &) = 0;
    virtual void checkIsComplete() {};
    bool complete = false;
};

