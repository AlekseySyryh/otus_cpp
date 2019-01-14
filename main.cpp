#include <cstdio>
#include "hash_factory.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <boost/unordered_set.hpp>


int main() {
    using namespace boost::container;
    using namespace boost::filesystem;
    using namespace boost;
    vector<path> files;
    unordered_set<path> exclusions;
    path p1{canonical("..")};
    exclusions.emplace(canonical("."));
    recursive_directory_iterator it(p1), end;
    while (it != end) {
        file_status fs = it->status();
        if (is_regular_file(fs)) {
            files.emplace_back(it->path());
        } else if (it.depth() > 0 ||
                   exclusions.count(it->path()) > 0) {
            it.no_push();
        }
        ++it;
    }
    for (auto &p : files) {
        std::cout << p << std::endl;
    }

    hashFactory factory;
    boost::container::vector<unsigned char> buff{1, 2, 3, 4, 5, 6};
    boost::container::vector<unsigned char> buff2{1, 2, 3, 4, 5, 6};
    boost::container::vector<unsigned char> buff3{1, 2, 3, 4, 5, 6, 7};
    auto algs = factory.getAlgorythms();
    for (auto &alg : algs) {
        std::cout << std::endl << alg << std::endl;
        auto ret = factory.getFunction(alg)(buff);
        BOOST_FOREACH(int c, ret) {
                        std::cout << std::hex << c;
                    }
        std::cout << std::endl;
        ret = factory.getFunction(alg)(buff2);
        BOOST_FOREACH(int c, ret) {
                        std::cout << std::hex << c;
                    }
        std::cout << std::endl;
        ret = factory.getFunction(alg)(buff3);
        BOOST_FOREACH(int c, ret) {
                        std::cout << std::hex << c;
                    }
    }
    return EXIT_SUCCESS;
}