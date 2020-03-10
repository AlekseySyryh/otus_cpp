#pragma once

#include "block.h"

class NotifyReceivers {
public:
    virtual void notify(std::shared_ptr<const Block> blockToProcess) const = 0;
};