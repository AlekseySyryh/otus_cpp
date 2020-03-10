#pragma once

#include <tuple>
#include <functional>

class Block {
public:
    Block() {
        nextBlockFunc = []() ->
                std::shared_ptr<Block> {
            throw std::logic_error("Тип следующего блока не определен, так как блок еще не закончен.");
        };
    }

    void addCommand(std::string &command) {
        if (command == "{" || command == "}") {
            processSpecialCommand(command);
        } else {
            if (commands.empty()) {
                time = std::time(NULL);
            }
            commands.push_back(command);
            checkIsComplete();
        }
    }

    std::time_t getTime() const {
        if (commands.empty()) {
            throw std::logic_error("Блок пуст - времени нет");
        }
        return time;
    }

    const std::vector<std::string> &getCommands() const {
        return commands;
    }

    virtual void close() {}

    bool isComplete() const {
        return complete;
    }

    std::function<std::shared_ptr<Block>()> getNextBlockBuilder() const {
        return nextBlockFunc;
    }

protected:
    std::function<std::shared_ptr<Block>()> nextBlockFunc;

    virtual void processSpecialCommand(std::string &) = 0;

    virtual void checkIsComplete() {};
    bool complete = false;
    std::vector<std::string> commands{};
private:
    std::time_t time{};
};

