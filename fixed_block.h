#pragma once

#include "block.h"
#include "block_builder.h"
#include <shared_mutex>
#include <mutex>

class FixedBlock : public Block {
public:
    FixedBlock(size_t blockSize, std::shared_ptr<BlockBuilder> blockBuilder) :
            blockSize(blockSize),
            blockBuilder(blockBuilder) {
    }

    void close() override {
        std::unique_lock lock(commandsMutex);
        if (!commands.empty()) {
            complete = true;
            nextBlockFunc = blockBuilder->buildFixedBlock();
        }
    }

    void clear() override {
        std::unique_lock lock(commandsMutex);
        commands.clear();
    }

    const std::vector<std::string> &getCommands() const override {
        std::unique_lock lock(commandsMutex);
        return commands;
    }

    size_t getNumberOfCommands() const override {
        std::shared_lock lock(commandsMutex);
        return commands.size();
    }

    time_t getTime() const override {
        std::shared_lock lock(commandsMutex);
        if (commands.empty()) {
            throw std::logic_error("Блок пуст - времени нет");
        }
        return time;
    }

protected:
    void processRegularCommand(std::string &command) override {
        {
            std::unique_lock lock(commandsMutex);
            if (commands.empty()) {
                time = std::time(NULL);
            }
            commands.push_back(command);
        }
        checkIsComplete();
    }

    void processSpecialCommand(std::string &command) override {
        if (command == "{") {
            complete = true;
            nextBlockFunc = blockBuilder->buildDynamicBlock();
        }
    }

    void checkIsComplete() override {
        if (commands.size() >= blockSize) {
            close();
        }
    };
private:
    static std::vector<std::string> commands;
    mutable std::shared_timed_mutex commandsMutex;
    size_t blockSize;
    std::shared_ptr<BlockBuilder> blockBuilder;
    static std::time_t time;
};

std::vector<std::string> FixedBlock::commands{};
std::time_t FixedBlock::time{};



std::function<std::shared_ptr<Block>()> BlockBuilder::buildFixedBlock() {
    return std::function<std::shared_ptr<Block>()>([this]() {
        return std::make_shared<FixedBlock>(blockSize, shared_from_this());
    });
}
