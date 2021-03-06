#pragma once

#include "worker.h"
#include <sstream>
#include <fstream>

class FileWorker : Worker {
public:
    explicit FileWorker(size_t workersNo) : Worker(workersNo) {
    }

    std::string getName() override {
        return "file";
    }

    void processBlock(std::shared_ptr<const Block> blk, size_t) override {
        if (blk->getCommands().empty()) {
            return;
        }
        std::ofstream fs;
        std::stringstream ss;
        ss << "bulk" << blk->getTime() << "." << workerId << "." << ++blk_id << ".log";
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
private:
    size_t blk_id = 0;
};