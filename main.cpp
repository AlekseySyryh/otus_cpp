#include <cstdio>

#ifdef WITH_OPENSSL

#include <openssl/sha.h>
#include <openssl/md4.h>
#include <openssl/md5.h>

#endif

#include <boost/functional/hash.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <boost/container/vector.hpp>
#include <boost/unordered_set.hpp>
#include <boost/crc.hpp>

int main() {
    using namespace boost::container;
    using namespace boost::filesystem;
    using namespace boost;

    vector<path> files;
    unordered_set<path> exclusions;
    path p1{"/mnt/c/Users/asyry/Desktop"};
    exclusions.emplace("/mnt/c/Users/asyry/Desktop/Shell");
    recursive_directory_iterator it(p1), end;
    while (it != end) {
        if (it.depth() >= 1 ||
            exclusions.count(it->path()) > 0) {
            it.no_push();
        }
        file_status fs = it->status();
        if (is_regular_file(fs)) {
            files.emplace_back(it->path());
        }
        ++it;
    }
    for (auto &p : files) {
        std::cout << p << std::endl;
    }
    unsigned char ibuf[] = "compute hash";
#ifdef WITH_OPENSSL
    unsigned char obuf[SHA512_DIGEST_LENGTH];
    int i;

    {
        printf("\nSHA1\n");
        SHA1(ibuf, 12, obuf);
        for (i = 0; i < SHA_DIGEST_LENGTH; i++) {
            printf("%02x", obuf[i]);
        }
    }
    {
        printf("\nSHA224\n");
        SHA224(ibuf, 12, obuf);
        for (i = 0; i < SHA224_DIGEST_LENGTH; i++) {
            printf("%02x", obuf[i]);
        }
    }
    {
        printf("\nSHA256\n");
        SHA256(ibuf, 12, obuf);
        for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            printf("%02x", obuf[i]);
        }
    }
    printf("\nSHA384\n");
    SHA384(ibuf, 12, obuf);
    for (i = 0; i < SHA384_DIGEST_LENGTH; i++) {
        printf("%02x", obuf[i]);
    }
    printf("\nSHA512\n");
    SHA512(ibuf, 12, obuf);
    for (i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        printf("%02x", obuf[i]);
    }
    printf("\nMD4\n");
    MD4(ibuf, 12, obuf);
    for (i = 0; i < MD4_DIGEST_LENGTH; i++) {
        printf("%02x", obuf[i]);
    }
    printf("\nMD5\n");
    MD5(ibuf, 12, obuf);
    for (i = 0; i < MD5_DIGEST_LENGTH; i++) {
        printf("%02x", obuf[i]);
    }
#endif
    printf("\nTR1\n");
    hash<unsigned char *> string_hash;
    std::size_t h = string_hash(ibuf);
    std::cout << h;
    boost::crc_32_type result;
    result.process_bytes(ibuf, 12);
    printf("\nCRC\n");
    std::cout << result.checksum();
    return EXIT_SUCCESS;
}