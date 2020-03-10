#include <sstream>
#include <cmath>
#include <algorithm>
#include "common.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Порядок запуска:\n"
                     "# rclss modelname\n"
                     "где modelname - название модели." << std::endl;
        return EXIT_FAILURE;
    }
    std::string file = argv[1];
    sample_type sample;
    for (size_t i = 0; i < 6; ++i) {
        std::cin >> sample(i);
        std::cin.ignore();
    }
    dlib::decision_function<kernel_type> df;
    try {
        dlib::deserialize(file) >> df;
    } catch (dlib::serialization_error) {
        std::cerr << "Файл модели не найден" << std::endl;
        return EXIT_FAILURE;
    }
    size_t clId = std::round(df(sample));
    std::ifstream cldata(file + "." + std::to_string(clId));
    if (cldata.fail()) {
        std::cerr << "Неизвестный кластер " << clId << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<sample_type> cluster;
    for (std::string line; std::getline(cldata, line);) {
        sample_type rec;
        std::istringstream lines(line);
        for (size_t ix = 0; ix <= 6; ++ix) {
            double d;
            lines >> d;
            lines.seekg(1, lines.cur);
            rec(ix) = d;
        }
        cluster.push_back(rec);
    }
    std::sort(cluster.begin(), cluster.end(),
              [&sample](const auto &lhs, const auto &rhs) {
                  return std::sqrt(
                          (lhs(0) - sample(0)) * (lhs(0) - sample(0)) + (lhs(1) - sample(1)) * (lhs(1) - sample(1))) <
                         std::sqrt((rhs(0) - sample(0)) * (rhs(0) - sample(0)) +
                                   (rhs(1) - sample(1)) * (rhs(1) - sample(1)));
              });
    std::for_each(cluster.begin(), cluster.end(),
                  [](const auto &rec) {
                      for (size_t ix = 0; ix < 7; ++ix) {
                          std::cout << std::fixed << std::setprecision(precs[ix]) << rec(ix);
                          if (ix == 6) {
                              std::cout << std::endl;
                          } else {
                              std::cout << ";";
                          }
                      }
                  });
    return EXIT_SUCCESS;
}