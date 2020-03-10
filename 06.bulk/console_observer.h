#pragma once

#include "observer.h"

struct ConsoleObserver : Observer {
    void process(std::shared_ptr<const Block> blk) const override {
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

