#pragma once

#include "observer.h"

class FileObserver : public Observer {
public:
    explicit FileObserver(size_t workersNo) : Observer(workersNo) {
        name = "file";
    }

private:
    void processBlock(std::shared_ptr<const Block> blk, size_t workerId) const override {
        if (blk->getCommands().empty()) {
            return;
        }
        std::ofstream fs;
        std::stringstream ss;
        ss << "bulk" << blk->getTime() << "." << workerId << ".log";
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