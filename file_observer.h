#pragma once

#include "observer.h"

struct FileObserver : Observer {
    void process(std::shared_ptr<const Block> blk) const override {
        if (blk->getCommands().empty()) {
            return;
        }
        std::ofstream fs;
        std::stringstream ss;
        ss << "bulk" << blk->getTime() << ".log";
        fs.open(ss.str());

        bool first = true;
        std::for_each(blk->getCommands().begin(), blk->getCommands().end(),
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