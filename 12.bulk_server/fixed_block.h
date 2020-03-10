#pragma once

#include "block.h"

class FixedBlock : public Block {
public:
    FixedBlock(size_t blockSize) :
            blockSize(blockSize) {
    }

    void close() override {
        if (!commands.empty()) {
            complete = true;
        }
    }

protected:
    void processSpecialCommand(const std::string &command) override {
        if (command == "{") {
            complete = true;
        }
    }

    void checkIsComplete() override {
        if (commands.size() >= blockSize) {
            close();
        }
    };
private:
    size_t blockSize;
};