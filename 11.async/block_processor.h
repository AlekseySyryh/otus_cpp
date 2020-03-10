#pragma once

#include "block_builder.h"
#include "terminal_block.h"


class BlockProcessor {
public:
    BlockProcessor(std::shared_ptr<NotifyReceivers> notifyReceivers,
                   std::shared_ptr<BlockBuilder> blockBuilder, size_t clientId) :
            clientId(clientId),
            notifyReceivers(notifyReceivers),
            blockBuilder(blockBuilder) {
        currentBlock = blockBuilder->buildDefaultBlock()();
    }

    ~BlockProcessor() {
        currentBlock->close();
        if (currentBlock->isComplete()) {
            ++blocks;
            commands += currentBlock->getNumberOfCommands();
            notify();
        }
        std::lock_guard<std::mutex> consoleLock(consoleMutex);
        std::cout << "main " << clientId << " поток - " << lines << " строк, " << blocks << " блоков, " << commands
                  << " команд"
                  << std::endl;
    }

    void processCommand(std::string &command) {
        ++lines;
        currentBlock->addCommand(command);
        if (currentBlock->isComplete()) {
            size_t numberOfCommands = currentBlock->getNumberOfCommands();
            if (numberOfCommands > 0) {
                ++blocks;
                commands += numberOfCommands;
                notify();
            }
            currentBlock = currentBlock->getNextBlockBuilder()();
        }
    }

private:
    size_t lines = 0, blocks = 0, commands = 0, clientId;
    std::shared_ptr<Block> currentBlock;
    std::shared_ptr<NotifyReceivers> notifyReceivers;
    std::shared_ptr<BlockBuilder> blockBuilder;

    void notify() {
        auto ptr = std::const_pointer_cast<const Block>(currentBlock);
        notifyReceivers->notify(ptr);
    }
};