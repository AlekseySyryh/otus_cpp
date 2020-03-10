#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>
#include "common.h"


std::vector<sample_type> prepareData() {
    std::vector<sample_type> recs;
    for (std::string line; std::getline(std::cin, line);) {
        sample_type rec;
        std::istringstream lines(line);
        for (size_t ix = 0; ix <= 5; ++ix) {
            double d;
            lines >> d;
            lines.seekg(1, lines.cur);
            rec(ix) = d;
        }
        size_t l1, l2;
        lines >> l1;
        lines.seekg(1, lines.cur);
        lines >> l2;
        rec(6) = (l1 == l2);
        recs.push_back(rec);
    }
    return recs;
}

void process(const std::string &fileName, size_t clusters) {
    std::vector<sample_type> recs = prepareData();
    std::vector<sample_type> initial_centers;
    dlib::kcentroid<kernel_type> kc(kernel_type(), 0.01, 8);
    dlib::kkmeans<kernel_type> test(kc);
    test.set_number_of_centers(clusters);
    pick_initial_centers(clusters, initial_centers, recs, test.get_kernel());
    test.train(recs, initial_centers);
    std::vector<double> classes;
    classes.reserve(recs.size());
    for (size_t i = 0; i < recs.size(); ++i) {
        classes.push_back(test(recs[i]));
    }
    randomize_samples(recs, classes);
    dlib::krr_trainer<kernel_type> trainer;
    trainer.set_kernel(kernel_type());
    dlib::decision_function<kernel_type> df = trainer.train(recs, classes);

    dlib::serialize(fileName) << df;

    std::vector<std::ofstream> outstreams;
    outstreams.reserve(clusters);
    for (size_t ix = 0; ix < clusters; ++ix) {
        outstreams.emplace_back(fileName + "." + std::to_string(ix));
    }
    for (size_t i = 0; i < recs.size(); ++i) {
        for (size_t j = 0; j < 7; ++j) {
            outstreams[classes[i]] << std::setprecision(std::numeric_limits<long double>::digits10 + 1) <<
                                   recs[i](j);
            if (j == 6) {
                outstreams[classes[i]] << std::endl;
            } else {
                outstreams[classes[i]] << ";";
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Порядок запуска:\n"
                     "# rclst clst modelname\n"
                     "где clst - количество кластеров\n"
                     "modelname - название модели." << std::endl;
        return EXIT_FAILURE;
    }
    std::istringstream ss(argv[1]);
    size_t clusters;
    ss >> clusters;
    if (ss.fail()) {
        std::cerr << "Не могу разобрать количество кластеров" << std::endl;
        return EXIT_FAILURE;
    }
    std::string fileName = argv[2];
    process(fileName, clusters);
    return EXIT_SUCCESS;
}

