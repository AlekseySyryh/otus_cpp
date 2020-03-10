#pragma once


#include <boost/tuple/tuple.hpp>
#include <istream>

class fileDescriptor {
public:
    fileDescriptor(const boost::filesystem::path &path,
                   const options &opt,
                   const boost::function<boost::container::vector<unsigned char>(
                           boost::container::vector<unsigned char> &)> &hashFunc) :
            path(path), fileSize(file_size(path)), opt(opt), hashFunc(hashFunc) {
        size_t blocks = (fileSize + opt.blockSize - 1) / opt.blockSize;
        hashes.resize(blocks);
    }

    const boost::container::vector<unsigned char> &getBlock(size_t block) const {
        if (block >= hashes.size()) {
            throw std::logic_error("");
        }
        if (!hashes[block].is_initialized()) {
            if (opt.debug) {
                std::cout << "Читаем файл " << path.filename().string() << " блок " << block << std::endl;
            }
            std::ifstream f;
            f.rdbuf()->pubsetbuf(0,0);
            f.open(path.string());
            f.seekg(block * opt.blockSize, std::ios::beg);

            boost::container::vector<unsigned char> buf(opt.blockSize);

            f.read(reinterpret_cast<char *>(buf.data()), buf.size());

            hashes[block] = hashFunc(buf);
        }
        return hashes[block].get();
    }

    const std::string &getName() const {
        return path.string();
    }

    size_t getSize() const {
        return hashes.size();
    }

    bool operator<(const fileDescriptor &other) const {
        if (getName() == other.getName()) {
            return false;
        }
        if (fileSize != other.fileSize) {
            return fileSize < other.fileSize;
        }
        auto len = getSize();
        for (size_t i = 0; i < len; ++i) {
            const auto &my = getBlock(i);
            const auto &their = other.getBlock(i);
            if (my != their) {
                return my < their;
            }
        }
        return false;
    }
private:
    mutable boost::container::vector<boost::optional<boost::container::vector<unsigned char>>> hashes;
    const boost::filesystem::path path;
    const size_t fileSize;
    const options &opt;
    const boost::function<boost::container::vector<unsigned char>(boost::container::vector<unsigned char> &)> &hashFunc;
};
