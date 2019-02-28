#include <iostream>
#include <sstream>
#include <cassert>
#include "yamr.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Порядок запуска:\n"
                     "# yamr <src> <mnum> <rnum>\n"
                     "где\n"
                     "• src – путь к файлу с данными.\n"
                     "• mnum – количество потоков для работы отображения.\n"
                     "• rnum – количество потоков для работы свертки." << std::endl;
        return EXIT_FAILURE;
    }
    std::string src = argv[1];
    std::istringstream ss(argv[2]);
    size_t mnum;
    ss >> mnum;
    if (ss.bad() || mnum == 0) {
        std::cerr << "Не могу разобрать mnum" << std::endl;
        return EXIT_FAILURE;
    }
    ss = std::istringstream(argv[3]);
    size_t rnum;
    ss >> rnum;
    if (ss.bad() || rnum == 0) {
        std::cerr << "Не могу разобрать rnum" << std::endl;
        return EXIT_FAILURE;
    }

    yamr<std::string, char, std::string, size_t>
            mr(mnum, rnum,
               [](const auto &str) {//Map
                   return std::set<std::pair<char, std::string>>{{str[0], str}};
               },
               [](const auto &chr) {//MapKeyHashFunction
                   return chr;
               },
               [](const auto &data) {//Reduce
                   size_t maxPrefix = 1;
                   auto it = data.begin();
                   while (it != data.end() && it + 1 != data.end()) {
                       auto next = it + 1;
                       auto diff = std::mismatch(
                               it->second.begin(), it->second.end(),
                               next->second.begin(), next->second.end());
                       auto dist = diff.first - it->second.begin() + 1;

                       assert(dist > 0);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
                       if (dist > maxPrefix) {
                           maxPrefix = dist;
                       }
#pragma GCC diagnostic pop

                       ++it;
                   }
                   return std::vector<size_t>{maxPrefix};
               });
    try {
        mr.process(src);
    }
    catch (const std::logic_error &er) {
        std::cerr << er.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
