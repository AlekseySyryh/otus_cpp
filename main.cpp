#include <iostream>
#include <ctime>
#include <limits>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>
#include <sstream>

enum class blockType {
    fixed,
    dynamic
};

struct block {
    std::time_t startTime;
    std::vector<std::string> commands;
};

struct observer {
    virtual void process(std::shared_ptr<block> blk) = 0;
};

struct consoleObserver : observer {
    void process(std::shared_ptr<block> blk) override {
        if (blk->commands.empty()) {
            return;
        }
        std::cout << "bulk: ";
        bool first = true;
        std::for_each(blk->commands.begin(), blk->commands.end(),
                      [&first](const std::string &command) {
                          if (!first) {
                              std::cout << ",";
                          }
                          first = false;
                          std::cout << " " << command;
                      });
        std::cout << std::endl;
    }
};

struct fileObserver : observer {
    void process(std::shared_ptr<block> blk) override {
        if (blk->commands.empty()) {
            return;
        }
        std::ofstream fs;
        std::stringstream ss;
        ss << "bulk" << blk->startTime << ".log";
        fs.open(ss.str());

        bool first = true;
        std::for_each(blk->commands.begin(), blk->commands.end(),
                      [&first, &fs](const std::string &command) {
                          if (!first) {
                              fs << ", ";
                          }
                          first = false;
                          fs << command;
                      });
        fs << std::endl;
        fs.close();
    }
};

class commandProcessor {
public:
    explicit commandProcessor(size_t blkSize) : maxBlockSize(blkSize), currentBlockType(blockType::fixed),
                                                blockLevel(0) {
        currentBlock = std::make_shared<block>();
    }

    void subscribe(std::unique_ptr<observer> &&obs) {
        observers.push_back(std::move(obs));
    }

    void addCommand(const std::string &cmd) {
        if (cmd == "{") {
            if (currentBlockType == blockType::fixed) {
                createNewBlock();
                currentBlockType = blockType::dynamic;
            } else {
                ++blockLevel;
            }

        } else if (cmd == "}") {
            if (currentBlockType == blockType::dynamic) {
                if (blockLevel == 0) {
                    createNewBlock();
                    currentBlockType = blockType::fixed;
                } else {
                    --blockLevel;
                }
            }
        } else {
            currentBlock->commands.push_back(cmd);
            if (currentBlock->commands.size() == 1) {
                currentBlock->startTime = std::time(nullptr);
            }
            if (currentBlockType == blockType::fixed && currentBlock->commands.size() == maxBlockSize) {
                createNewBlock();
            }
        }
    }

    ~commandProcessor() {
        if (currentBlockType == blockType::fixed && !currentBlock->commands.empty()) {
            processBlock(currentBlock);
        }
    }

private:
    void createNewBlock() {
        processBlock(currentBlock);
        blockLevel = 0;
        currentBlock = std::make_shared<block>();

    }

    void processBlock(const std::shared_ptr<block> &blockToProcess) {
        std::for_each(
                observers.begin(),
                observers.end(),
                [blockToProcess](std::unique_ptr<observer> &obs) {
                    obs->process(blockToProcess);
                });
    }

    size_t blockLevel;
    size_t maxBlockSize;
    blockType currentBlockType;
    std::shared_ptr<block> currentBlock;
    std::vector<std::unique_ptr<observer>> observers;
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Следует указать единственный параметр - размер блока" << std::endl;
        return 1;
    }
    size_t block_size;
    const size_t max_block_size = 1000000000;
    try {
        block_size = std::stoul(argv[1]);
        if (block_size > max_block_size) {
            throw std::out_of_range("");
        }
    }
    catch (std::logic_error &) {
        std::cerr << "Размер блока должен быть целым, положительным числом, не более "
                  << max_block_size << std::endl;
        return 1;
    }

    std::string line;
    commandProcessor processor(block_size);
    processor.subscribe(std::make_unique<consoleObserver>());
    processor.subscribe(std::make_unique<fileObserver>());
    while (std::getline(std::cin, line)) {
        processor.addCommand(line);
    }
    return 0;
}

