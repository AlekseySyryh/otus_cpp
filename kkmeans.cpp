#include <iostream>
#include <sstream>
#include <fstream>
#include <dlib/clustering.h>


typedef dlib::matrix<double, 2, 1> sample_type;
typedef dlib::polynomial_kernel<sample_type> kernel_type;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Порядок запуска:\n"
                     "# kkmeans n\n"
                     "где n - количество кластеров."
                  << std::endl;
        return EXIT_FAILURE;
    }
    size_t nclust;
    std::istringstream ss(argv[1]);
    ss >> nclust;
    if (ss.fail() || nclust == 0) {
        std::cerr << "Неверное количество кластеров."
                  << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<sample_type> samples;
    for (std::string line; std::getline(std::cin, line);) {
        sample_type m;
        std::istringstream ss(line);
        ss >> m(0);
        ss.seekg(1, ss.cur);
        ss >> m(1);
        samples.push_back(m);
    }
    std::vector<sample_type> initial_centers;
    dlib::kcentroid<kernel_type> kc(kernel_type(), 0.01, 8);
    dlib::kkmeans<kernel_type> test(kc);
    test.set_number_of_centers(nclust);
    pick_initial_centers(3, initial_centers, samples, test.get_kernel());
    test.train(samples, initial_centers);
    for (unsigned long i = 0; i < samples.size(); ++i) {
        std::cout << samples[i](0) << ";" << samples[i](1) << ";cluster" << test(samples[i]) + 1 << std::endl;
    }
    return EXIT_SUCCESS;
}