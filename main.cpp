#include <cstdio>
#include "hash_factory.h"
#include "options.h"
#include <boost/iostreams/device/file.hpp>
#include "file_descriptor.h"
#include <boost/iterator/distance.hpp>

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
                        } else if (is_directory(fs) &&
                                   (it.depth() >= opt.deep ||
                                    opt.dirsToExclude.count(it->path()))) {
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

    auto hashFunc = factory.getFunction(opt.algorithm);

    boost::container::multiset<fileDescriptor> fds;

    BOOST_FOREACH(auto &p, files) {
                    if (opt.debug) {
                        std::cout << "Добавляем файл " << p << std::endl;
                    }
                    fds.emplace(p, opt, hashFunc);
                }

    auto it = fds.begin();
    while (it != fds.end()) {
        auto er = fds.equal_range(*it);
        auto dist = boost::distance(er.first, er.second);
        if (dist > 1) {
            while (it != er.second) {
                std::cout << it->getName() << std::endl;
                ++it;
            }
            std::cout << std::endl;
        } else {
            ++it;
        }
    }

    return EXIT_SUCCESS;
}