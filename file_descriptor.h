#pragma once

#include <boost/algorithm/minmax.hpp>
#include <boost/tuple/tuple.hpp>

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
            boost::iostreams::file_source in(path.string(), BOOST_IOS::binary);
            boost::iostreams::seek(in, block * opt.blockSize, BOOST_IOS::beg);
            boost::container::vector<unsigned char> buf(opt.blockSize);
            in.read(reinterpret_cast<char *>(buf.data()), buf.size());
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
        auto t = boost::minmax(getSize(), other.getSize());
        auto minLen = t.get<0>();
        for (size_t i = 0; i < minLen; ++i) {
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
