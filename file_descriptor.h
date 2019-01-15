#pragma once

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

    const boost::container::vector<unsigned char> &getBlock(size_t block) {
        if (block >= hashes.size()) {
            throw new std::logic_error("");
        }
        if (!hashes[block].is_initialized()) {
            boost::iostreams::file_source in(path.string(), BOOST_IOS::binary);
            boost::iostreams::seek(in, block * opt.blocksize, BOOST_IOS::beg);
            boost::container::vector<unsigned char> buf(opt.blocksize);
            in.read(reinterpret_cast<char *>(buf.data()), opt.blocksize);
            hashes[block] = hashFunc(buf);
        }
        return hashes[block].get();
    }

    size_t getSize() {
        return hashes.size();
    }

private:
    boost::container::vector<boost::optional<boost::container::vector<unsigned char>>> hashes;
    const boost::filesystem::path path;
    const options &opt;
    boost::function<boost::container::vector<unsigned char>(boost::container::vector<unsigned char> &buff)> hashFunc;
};