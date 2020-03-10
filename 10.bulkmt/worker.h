#pragma once

#include "console_mutex.h"

class Worker {
public:
    explicit Worker(size_t workerId) : workerId(workerId) {}

    virtual std::string getName() = 0;

    virtual void processBlock(std::shared_ptr<const Block> blk, size_t) = 0;

protected:
    size_t workerId;
};