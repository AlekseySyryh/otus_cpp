#pragma once

#ifdef WITH_OPENSSL

#include <openssl/sha.h>
#include <openssl/md4.h>
#include <openssl/md5.h>

#endif

#include <boost/functional/hash.hpp>
#include <boost/crc.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/set.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>

template<typename T>
boost::container::vector<unsigned char> toVector(T value) {
    boost::container::vector<unsigned char> ret(sizeof(value));
    for (size_t i = 0; i < sizeof(value); ++i) {
        ret[i] = value % 256;
        value /= 256;
    }
    return ret;
}

boost::container::vector<unsigned char> getCrcHash(boost::container::vector<unsigned char> &buff) {
    boost::crc_32_type result;
    result.process_bytes(buff.data(), buff.size());
    auto res = result.checksum();
    return toVector(res);
}

boost::container::vector<unsigned char> getBoostHash(boost::container::vector<unsigned char> &buff) {
    boost::hash<std::string> hasher;
    std::size_t res = hasher(std::string((reinterpret_cast<char *>(buff.data())), buff.size()));
    return toVector(res);
}

#ifdef WITH_OPENSSL

boost::container::vector<unsigned char> getMD4Hash(boost::container::vector<unsigned char> &buff) {
    boost::container::vector<unsigned char> ret(MD4_DIGEST_LENGTH);
    MD4(buff.data(), buff.size(), ret.data());
    return ret;
}

boost::container::vector<unsigned char> getMD5Hash(boost::container::vector<unsigned char> &buff) {
    boost::container::vector<unsigned char> ret(MD5_DIGEST_LENGTH);
    MD5(buff.data(), buff.size(), ret.data());
    return ret;
}

boost::container::vector<unsigned char> getSHA1Hash(boost::container::vector<unsigned char> &buff) {
    boost::container::vector<unsigned char> ret(SHA_DIGEST_LENGTH);
    SHA1(buff.data(), buff.size(), ret.data());
    return ret;
}

boost::container::vector<unsigned char> getSHA224Hash(boost::container::vector<unsigned char> &buff) {
    boost::container::vector<unsigned char> ret(SHA224_DIGEST_LENGTH);
    SHA224(buff.data(), buff.size(), ret.data());
    return ret;
}

boost::container::vector<unsigned char> getSHA256Hash(boost::container::vector<unsigned char> &buff) {
    boost::container::vector<unsigned char> ret(SHA256_DIGEST_LENGTH);
    SHA256(buff.data(), buff.size(), ret.data());
    return ret;
}

boost::container::vector<unsigned char> getSHA384Hash(boost::container::vector<unsigned char> &buff) {
    boost::container::vector<unsigned char> ret(SHA384_DIGEST_LENGTH);
    SHA384(buff.data(), buff.size(), ret.data());
    return ret;
}

boost::container::vector<unsigned char> getSHA512Hash(boost::container::vector<unsigned char> &buff) {
    boost::container::vector<unsigned char> ret(SHA512_DIGEST_LENGTH);
    SHA512(buff.data(), buff.size(), ret.data());
    return ret;
}

#endif

class hashFactory {
public:
    hashFactory() noexcept {
        functions["CRC"] = getCrcHash;
        functions["BOOST"] = getBoostHash;
#ifdef WITH_OPENSSL
        functions["MD4"] = getMD4Hash;
        functions["MD5"] = getMD5Hash;
        functions["SHA1"] = getSHA1Hash;
        functions["SHA224"] = getSHA224Hash;
        functions["SHA256"] = getSHA256Hash;
        functions["SHA384"] = getSHA384Hash;
        functions["SHA512"] = getSHA512Hash;
#endif
    }

    auto getFunction(std::string &algorithm) const {
        return functions.at(algorithm);
    }

    auto getAlgorithms() const {
        boost::container::set<std::string> keys;
        BOOST_FOREACH(auto rec, functions) {
                        keys.insert(rec.first);
                    }
        return keys;
    }

private:
    boost::unordered_map<
            std::string,
            boost::function<boost::container::vector<unsigned char>(
                    boost::container::vector<unsigned char> &)>> functions;
};

static const hashFactory HashFactory;