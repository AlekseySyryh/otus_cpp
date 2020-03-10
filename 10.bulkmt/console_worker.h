#pragma once

#include "worker.h"

class ConsoleWorker : Worker {
public:
    explicit ConsoleWorker(size_t workerId) : Worker(workerId) {}

    std::string getName() override {
        return "log";
    }

    void processBlock(std::shared_ptr<const Block> blk, size_t) override {
        if (blk->getCommands().empty()) {
            return;
        }
        std::lock_guard<std::mutex> consoleLock(consoleMutex);
        std::cout << "bulk: ";
        bool first = true;
        std::for_each(blk->getCommands().begin(), blk->getCommands().end(),
                      [&first](const std::string &command) {
                          if (!first) {
                              std::cout << ",";
                          }
                          first = false;
                          std::cout << " " << command;
                      });
        std::cout << "\n";
    }
};

