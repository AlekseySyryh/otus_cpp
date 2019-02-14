#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <algorithm>
#include "async.h"
#include "observers.h"
#include "console_worker.h"
#include "file_worker.h"
#include "block_builder.h"
#include "block_processor.h"
#include "fixed_block.h"
#include "dynamic_block.h"
#include "terminal_block.h"

namespace async {

    static std::shared_ptr<Observers> obs = []() {
        auto obs = std::make_shared<Observers>();
        obs->add(std::make_unique<Observer<ConsoleWorker>>(1));
        obs->add(std::make_unique<Observer<FileWorker>>(2));
        return obs;
    }();

    struct client {
        client(std::string buffer, std::unique_ptr<BlockProcessor> proc) : buffer(buffer), proc(std::move(proc)) {

        }
        std::string buffer;
        std::unique_ptr<BlockProcessor> proc;
        std::mutex reenterMutex;
    };

    static size_t clientId;
    static std::unordered_map<size_t, std::shared_ptr<client>> clients;
    static std::shared_timed_mutex mutex;

    handle_t connect(std::size_t bulk) {
        std::shared_ptr<BlockBuilder> bb = std::make_shared<BlockBuilder>(bulk);
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
        size_t id = clientId++;
        clients[id] = std::make_shared<client>("", std::make_unique<BlockProcessor>(obs, bb, id));
        return reinterpret_cast<void *>(id);
}

    void receive(handle_t handle, const char *data, std::size_t size) {
        auto id = reinterpret_cast<size_t>(handle);
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
        std::shared_ptr<client> client = clients[id];
        std::lock_guard<std::mutex> reenterGuard(client->reenterMutex);
        client->buffer.append(data, size);
        std::string::iterator it;
        while ((it = std::find(client->buffer.begin(), client->buffer.end(), '\n')) != client->buffer.end()) {
            std::string str(client->buffer.begin(), it);
            client->proc->processCommand(str);
            client->buffer = std::string(it + 1, client->buffer.end());
        }
}

    void disconnect(handle_t handle) {
        auto id = reinterpret_cast<size_t>(handle);
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
        clients.erase(id);
}
}
