#pragma once

#include "terminal_block.h"
#include "block_type.h"
#include "dynamic_block.h"
#include "fixed_block.h"
#include <mutex>

class BlockProcessor {
public:
    BlockProcessor(std::shared_ptr<NotifyReceivers> notifyReceivers, size_t bulk, size_t clientId) :
            clientId(clientId),
            bulk(bulk),
            notifyReceivers(notifyReceivers) {
        std::lock_guard<std::mutex> guard(sharedBlockMutex);
        NextBlockIsFixed();
    }

    ~BlockProcessor() {
        std::lock_guard<std::mutex> consoleLock(consoleMutex);
        std::cout << "main " << clientId << " поток - " << lines << " строк, " << blocks << " блоков, " << commands
                  << " команд"
                  << std::endl;
    }

    void processCommand(const std::string &command) {
        ++lines;
        if (command == "{") {
            switch (currentBlockType) {
                case Fixed:
                    NextBlockIsDynamic();
                    break;
                case Dynamic:
                    currentBlock->addCommand(command);
                    break;
            }
        } else if (command == "}") {
            switch (currentBlockType) {
                case Fixed:
                    break;
                case Dynamic:
                    currentBlock->addCommand(command);
                    if (currentBlock->isComplete()) {
                        notify(currentBlock);
                        {
                            std::lock_guard<std::mutex> guard(sharedBlockMutex);
                            NextBlockIsFixed();
                        }

                    }
                    break;
            }
        } else {
            switch (currentBlockType) {
                case Fixed: {
                    std::lock_guard<std::mutex> guard(sharedBlockMutex);
                    addCommand(sharedFixedBlock, command);
                }
                    break;
                case Dynamic:
                    addCommand(currentBlock, command);
                    break;
            }
        }
    }


private:
    void NextBlockIsDynamic() {
        currentBlock = std::make_shared<DynamicBlock>();
        currentBlockType = BlockType::Dynamic;
    }

    void NextBlockIsFixed() {
        if (sharedFixedBlock == nullptr ||
            sharedFixedBlock->isComplete()) {
            sharedFixedBlock = std::make_shared<FixedBlock>(bulk);
        }
        currentBlockType = BlockType::Fixed;
    }

    void addCommand(std::shared_ptr<Block> &block, const std::string &command) {
        block->addCommand(command);
        if (block->isComplete()) {
            size_t numberOfCommands = block->getNumberOfCommands();
            if (numberOfCommands > 0) {
                ++blocks;
                commands += numberOfCommands;
                notify(block);
            }
            NextBlockIsFixed();
        }
    }

    void notify(const std::shared_ptr<Block> &ptr) {
        //auto ptr = std::const_pointer_cast<const Block>(sharedFixedBlock);
        notifyReceivers->notify(ptr);
    }

    size_t lines = 0, blocks = 0, commands = 0, clientId, bulk;
    std::shared_ptr<Block> currentBlock;
    std::shared_ptr<NotifyReceivers> notifyReceivers;
    static std::shared_ptr<Block> sharedFixedBlock;
    static std::mutex sharedBlockMutex;

    BlockType currentBlockType;

};

std::shared_ptr<Block> BlockProcessor::sharedFixedBlock = nullptr;
std::mutex BlockProcessor::sharedBlockMutex;


