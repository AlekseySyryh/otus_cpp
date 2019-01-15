#include <cstdio>
#include "hash_factory.h"
#include "options.h"
#include <boost/iostreams/device/file.hpp>

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
    BOOST_FOREACH(auto &p, files) {
                    std::cout << p << std::endl;
                    size_t blocks = (file_size(p) + opt.blocksize - 1) / opt.blocksize;
                    for (size_t bl = 0; bl < blocks; ++bl) {
                        std::cout << "Block " << bl + 1 << " of " << blocks << std::endl;
                        boost::iostreams::file_source in(p.string(), BOOST_IOS::binary);
                        boost::iostreams::seek(in, bl * opt.blocksize, BOOST_IOS::beg);
                        vector<unsigned char> buf(opt.blocksize);
                        in.read(reinterpret_cast<char *>(buf.data()), opt.blocksize);
                        auto ret = factory.getFunction(opt.algo)(buf);
                        std::cout << std::hex;
                        BOOST_FOREACH(int c, ret) {
                                        std::cout << c;
                                    }
                        std::cout << std::dec << std::endl;
                    }
                }
    return EXIT_SUCCESS;
}