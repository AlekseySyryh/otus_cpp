#pragma once

#include "block.h"
#include "block_builder.h"

class DynamicBlock : public Block {
public:
    DynamicBlock(std::shared_ptr<BlockBuilder> blockBuilder) :
            blockBuilder(blockBuilder), openedBrackets(1) {
    }

protected:
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
};

std::function<std::shared_ptr<Block>()> BlockBuilder::buildDynamicBlock() {
    return std::function<std::shared_ptr<Block>()>([this]() {
        return std::make_shared<DynamicBlock>(shared_from_this());
    });
}