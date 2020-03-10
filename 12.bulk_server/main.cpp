#include <iostream>

#include "server.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Порядок запуска:\n"
                     "# bulk_server <port> <bulk_size>\n"
                     "где\n"
                     "• port – номер tcp порта для входящих соединений.\n"
                     "• bulk_size – размер блока команд." << std::endl;
        return EXIT_FAILURE;
    }
    std::istringstream ss(argv[1]);
    unsigned int port;
    ss >> port;
    if (port < 1 || port > 65535) {
        std::cerr << "Порт должен быть числом от 1 до 65535" << std::endl;
        return EXIT_FAILURE;
    }
    const size_t max_block_size = 1000000000;
    size_t block_size;
    ss = std::istringstream(argv[2]);
    ss >> block_size;
    if (block_size == 0 || block_size > max_block_size) {
        std::cerr << "Размер блока должен быть целым, положительным числом, не более "
                  << max_block_size << std::endl;
        return EXIT_FAILURE;
    }
    server(port, block_size);
    return EXIT_SUCCESS;
}
