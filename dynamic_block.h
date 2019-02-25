#pragma once

#include "block.h"
#include "block_builder.h"

class DynamicBlock : public Block {
public:
    DynamicBlock(std::shared_ptr<BlockBuilder> blockBuilder) :
            openedBrackets(1), blockBuilder(blockBuilder) {
    }

    time_t getTime() const override {
        if (commands.empty()) {
            throw std::logic_error("Блок пуст - времени нет");
        }
        return time;
    }

    const std::vector<std::string> &getCommands() const override {
        return commands;
    }

    size_t getNumberOfCommands() const override {
        return commands.size();
    }

protected:
    void processRegularCommand(std::string &command) override {
        if (commands.empty()) {
            time = std::time(NULL);
        }
        commands.push_back(command);
        checkIsComplete();
    }

    void processSpecialCommand(std::string &command) override {
        if (command == "{") {
            ++openedBrackets;
        } else if (command == "}") {
            --openedBrackets;
            if (openedBrackets == 0) {
                complete = true;
                nextBlockFunc = blockBuilder->buildFixedBlock();
            }
        }
    }

private:
    size_t openedBrackets;
    std::shared_ptr<BlockBuilder> blockBuilder;
    std::time_t time{};
    std::vector<std::string> commands{};
};

std::function<std::shared_ptr<Block>()> BlockBuilder::buildDynamicBlock() {
    return std::function<std::shared_ptr<Block>()>([this]() {
        return std::make_shared<DynamicBlock>(shared_from_this());
    });
}