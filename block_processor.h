#pragma once

#include "block_builder.h"

class BlockProcessor {
public:
    BlockProcessor(std::shared_ptr<NotifyReceivers> notifyReceivers,
                   std::shared_ptr<BlockBuilder> blockBuilder) :
            notifyReceivers(notifyReceivers),
            blockBuilder(blockBuilder) {
        currentBlock = blockBuilder->buildDefaultBlock()();
    }

    ~BlockProcessor() {
        currentBlock->close();
        if (currentBlock->isComplete()) {
            notify();
        }
    }

    void processCommand(std::string &command) {
        currentBlock->addCommand(command);
        if (currentBlock->isComplete()) {
            notify();
            currentBlock = currentBlock->getNextBlockBuilder()();
        }
    }

private:
    std::shared_ptr<Block> currentBlock;
    std::shared_ptr<NotifyReceivers> notifyReceivers;
    std::shared_ptr<BlockBuilder> blockBuilder;

    void notify() {
        auto ptr = std::const_pointer_cast<const Block>(currentBlock);
        notifyReceivers->notify(ptr);
    }
};