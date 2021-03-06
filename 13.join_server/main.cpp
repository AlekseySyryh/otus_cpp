#include <iostream>
#include "server.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Порядок запуска:\n"
                     "# join_server <port>\n"
                     "где\n"
                     "• port – номер tcp порта для входящих соединений." << std::endl;
        return EXIT_FAILURE;
    }
    std::istringstream ss(argv[1]);
    unsigned short port;
    ss >> port;
    if (ss.fail() || port == 0) {
        std::cerr << "Порт должен быть числом от 1 до 65535" << std::endl;
        return EXIT_FAILURE;
    }
    server(port);
    return EXIT_SUCCESS;
}