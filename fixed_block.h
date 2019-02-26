#pragma once

#include "block.h"
#include "block_builder.h"

class FixedBlock : public Block {
public:
    FixedBlock(size_t blockSize, std::shared_ptr<BlockBuilder> blockBuilder) :
            blockSize(blockSize),
            blockBuilder(blockBuilder) {
    }

    void close() override {
        if (!commands.empty()) {
            complete = true;
            nextBlockFunc = blockBuilder->buildFixedBlock();
        }
    }

protected:
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
    size_t blockSize;
    std::shared_ptr<BlockBuilder> blockBuilder;
};

std::function<std::shared_ptr<Block>()> BlockBuilder::buildFixedBlock() {
    return std::function<std::shared_ptr<Block>()>([this]() {
        return std::make_shared<FixedBlock>(blockSize, shared_from_this());
    });
}
