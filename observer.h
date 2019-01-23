#pragma once

#include "block.h"
#include <queue>
#include <vector>
#include <condition_variable>
#include <future>

static std::mutex consoleMutex;

class Observer {
public:
    explicit Observer(size_t workersNo) {
        for (size_t i = 0; i < workersNo; ++i) {
            workers.push_back(std::async([this, i]() -> void {
                size_t blocks = 0, commands = 0;
                while (true) {
                    std::shared_ptr<const Block> data;
                    {
                        std::unique_lock<std::mutex> dataLock(dataLockMutex);
                        dataReady.wait(dataLock, [this] { return !blocksQueue.empty(); });
                        data = blocksQueue.front();
                        if (data->isTerminalBlock()) {
                            dataReady.notify_one(); //Прочитал сам - дай другим почитать!
                        } else {
                            blocksQueue.pop();
                        }
                    }
                    if (data->isTerminalBlock()) {
                        std::lock_guard<std::mutex> consoleLock(consoleMutex);
                        std::cout << name << i + 1 << " поток - " << blocks << " блоков, " << commands << " команд"
                                  << std::endl;
                        return;
                    }
                    ++blocks;
                    commands += data->getNumberOfCommands();
                    processBlock(data, i);
                }
            }));
        }
    }

    void process(std::shared_ptr<const Block> blk) {
        std::lock_guard<std::mutex> dataLock(dataLockMutex);
        blocksQueue.push(blk);
        dataReady.notify_one();
    }

protected:
    std::string name;
private:
    std::mutex dataLockMutex;
    std::condition_variable dataReady;
    std::queue<std::shared_ptr<const Block>> blocksQueue;
    std::vector<std::future<void>> workers;

    virtual void processBlock(std::shared_ptr<const Block> blk, size_t workerId) const = 0;
};