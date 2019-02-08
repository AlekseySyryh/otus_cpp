#pragma once

#include "block.h"
#include "observer.h"
#include "notify_recivers.h"
#include "terminal_block.h"

class Observers : public NotifyReceivers {
public:
    ~Observers() {
        notify(std::make_shared<const TerminalBlock>());
    }

    void add(std::unique_ptr<ObserverBase> &&obs) {
        observers.push_back(std::move(obs));
    }

    void notify(std::shared_ptr<const Block> blockToProcess) const override {
        std::for_each(
                observers.begin(),
                observers.end(),
                [blockToProcess](const auto &obs) {
                    obs->process(blockToProcess);
                });
    }

private:
    std::vector<std::unique_ptr<ObserverBase>> observers;
};

