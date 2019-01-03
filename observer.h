#pragma once

#include "block.h"

struct Observer {
    virtual void process(std::shared_ptr<const Block> blk) const = 0;
};