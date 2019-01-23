#include <iostream>
#include <ctime>
#include <limits>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>
#include <sstream>
#include "observers.h"
#include "console_observer.h"
#include "file_observer.h"
#include "block_builder.h"
#include "block_processor.h"
#include "fixed_block.h"
#include "dynamic_block.h"
#include "terminal_block.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Следует указать единственный параметр - размер блока" << std::endl;
        return EXIT_FAILURE;
    }

    const size_t max_block_size = 1000000000;
    size_t block_size;
    try {
        block_size = std::stoul(argv[1]);
        if (block_size > max_block_size) {
            throw std::out_of_range("");
        }
    }
    catch (std::logic_error &) {
        std::cerr << "Размер блока должен быть целым, положительным числом, не более "
                  << max_block_size << std::endl;
        return EXIT_FAILURE;
    }
    std::shared_ptr<Observers> obs = std::make_shared<Observers>();
    obs->add(std::make_unique<ConsoleObserver>(1));
    obs->add(std::make_unique<FileObserver>(2));

    std::shared_ptr<BlockBuilder> bb = std::make_shared<BlockBuilder>(block_size);


    BlockProcessor proc(obs, bb);

    std::string line;
    while (std::getline(std::cin, line)) {
        proc.processCommand(line);
    }
    return EXIT_SUCCESS;
}

