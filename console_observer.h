#pragma once

#include "observer.h"

class ConsoleObserver : public Observer {
public:
    explicit ConsoleObserver(size_t blockNo) : Observer(blockNo) {
        name = "log";
    }

private:
    void processBlock(std::shared_ptr<const Block> blk, size_t) const override {
        if (blk->getCommands().empty()) {
            return;
        }
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
        std::cout << std::endl;
    }

};

