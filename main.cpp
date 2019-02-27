#include <iostream>
#include <fstream>
#include <sstream>

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
    std::ifstream fs(src, std::ios::binary | std::ios::ate);
    std::cout << src << " " << fs.good() << " " << fs.tellg() << std::endl;
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
}