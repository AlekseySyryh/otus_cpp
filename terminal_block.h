#pragma once

#include "block.h"

class TerminalBlock : public Block {
    bool isTerminalBlock() const override {
        return true;
    }
};