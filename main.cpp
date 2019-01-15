#include <cstdio>
#include "hash_factory.h"
#include "options.h"

auto getFiles(const options &opt) {
    using namespace boost::container;
    using namespace boost::filesystem;
    vector<path> files;
    BOOST_FOREACH(auto dir, opt.dirsToScan) {
                    recursive_directory_iterator it(dir), end;
                    while (it != end) {
                        file_status fs = it->status();
                        if (is_regular_file(fs) &&
                            file_size(*it) >= opt.minsize) {

                            if (boost::algorithm::any_of(opt.masks,
                                                         [&it](auto &mask) {
                                                             return boost::regex_match(it->path().filename().string(),
                                                                                       mask);
                                                         })) {
                                files.emplace_back(it->path());
                            }
                        } else if (it.depth() >= opt.deep ||
                                   opt.dirsToExclude.count(it->path())) {
                            it.no_push();
                        }
                        ++it;
                    }
                }
    return files;
}

int main(int argc, const char *argv[]) {
    using namespace boost::container;
    using namespace boost::filesystem;
    using namespace boost;
    hashFactory factory;
    vector<path> dirs;
    options opt(argc, argv);
    if (opt.exit_fail) {
        return EXIT_FAILURE;
    }
    if (opt.exit_ok) {
        return EXIT_SUCCESS;
    }
    vector<path> files = getFiles(opt);
    if (opt.debug) {
        BOOST_FOREACH(auto &p, files) {
                        std::cout << p << std::endl;
                    }
    }

    boost::container::vector<unsigned char> buff{1, 2, 3, 4, 5, 6};
    boost::container::vector<unsigned char> buff2{1, 2, 3, 4, 5, 6};
    boost::container::vector<unsigned char> buff3{1, 2, 3, 4, 5, 6, 7};

    std::cout << std::endl << opt.algo << std::endl;
    auto ret = factory.getFunction(opt.algo)(buff);
    BOOST_FOREACH(int c, ret) {
                    std::cout << std::hex << c;
                }
    std::cout << std::endl;
    ret = factory.getFunction(opt.algo)(buff2);
    BOOST_FOREACH(int c, ret) {
                    std::cout << std::hex << c;
                }
    std::cout << std::endl;
    ret = factory.getFunction(opt.algo)(buff3);
    BOOST_FOREACH(int c, ret) {
                    std::cout << std::hex << c;
                }

    return EXIT_SUCCESS;
}