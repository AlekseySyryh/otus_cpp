#pragma once

#include "block.h"

class DynamicBlock : public Block {
public:
    DynamicBlock() : openedBrackets(1) {
    }

protected:
    void processSpecialCommand(const std::string &command) override {
        if (command == "{") {
            ++openedBrackets;
        } else if (command == "}") {
            --openedBrackets;
            if (openedBrackets == 0) {
                complete = true;
            }
        }
    }

private:
    size_t openedBrackets;
};
