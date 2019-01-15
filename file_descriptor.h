#pragma once

#include <boost/algorithm/minmax.hpp>
#include <boost/tuple/tuple.hpp>

class fileDescriptor {
public:
    fileDescriptor(boost::filesystem::path path,
                   const options &opt,
                   boost::function<boost::container::vector<unsigned char>(
                           boost::container::vector<unsigned char> &buff)> hashFunc) :
            path(path), opt(opt), hashFunc(hashFunc) {
        size_t blocks = (file_size(path) + opt.blocksize - 1) / opt.blocksize;
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
            boost::iostreams::seek(in, block * opt.blocksize, BOOST_IOS::beg);
            boost::container::vector<unsigned char> buf(opt.blocksize);
            in.read(reinterpret_cast<char *>(buf.data()), opt.blocksize);
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
        boost::tuples::tuple<const int &, const int &> t = boost::minmax(getSize(), other.getSize());
        auto minLen = t.get<0>();
        for (size_t i = 0; i < minLen; ++i) {
            auto my = getBlock(i);
            auto their = other.getBlock(i);
            if (my != their) {
                return my < their;
            }
        }
        return getSize() < other.getSize();
    }

    bool operator==(const fileDescriptor &other) const {
        if (getSize() != other.getSize()) {
            return false;
        }
        for (size_t i = 0; i < getSize(); ++i) {
            auto my = getBlock(i);
            auto their = other.getBlock(i);
            if (my != their) {
                return false;
            }
        }
        return true;
    }
private:
    mutable boost::container::vector<boost::optional<boost::container::vector<unsigned char>>> hashes;
    const boost::filesystem::path path;
    const options &opt;
    boost::function<boost::container::vector<unsigned char>(boost::container::vector<unsigned char> &buff)> hashFunc;
};