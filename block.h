#pragma once

#include <tuple>
#include <functional>
#include <vector>

class Block {
public:
    virtual bool isTerminalBlock() const {
        return false;
    }

    void addCommand(const std::string &command) {
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

    size_t getNumberOfCommands() const {
        return commands.size();
    }

    virtual void close() {}

    bool isComplete() const {
        return complete;
    }

protected:
    virtual void processSpecialCommand(const std::string &) {};
    virtual void checkIsComplete() {};
    bool complete = false;
    std::vector<std::string> commands{};
private:
    std::time_t time{};
};

