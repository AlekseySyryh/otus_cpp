#pragma once

#include "block.h"

class BlockBuilder : public std::enable_shared_from_this<BlockBuilder> {
public:
    BlockBuilder(size_t blockSize) : blockSize(blockSize) {}

    std::function<std::shared_ptr<Block>()> buildDefaultBlock() {
        return buildFixedBlock();
    }

    std::function<std::shared_ptr<Block>()> buildFixedBlock();

    std::function<std::shared_ptr<Block>()> buildDynamicBlock();

private:
    size_t blockSize;
};



