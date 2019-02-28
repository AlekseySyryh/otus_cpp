#pragma once

#include <vector>
#include <functional>
#include <fstream>
#include <set>
#include <algorithm>
#include <future>
#include <list>
#include <optional>

template<typename InType,
        typename MapKeyType,
        typename MapValueType,
        typename ReduceValueType>
class yamr {
public:

    yamr(size_t mnum,
         size_t rnum,
         std::function<std::set<std::pair<MapKeyType, MapValueType>>(const InType &)> mapFunction,
         std::function<size_t(const MapKeyType &)> keyHashFunction,
         std::function<std::vector<ReduceValueType>(
                 const std::vector<std::pair<MapKeyType, MapValueType>> &)> reduceFunction) :
            mnum(mnum),
            rnum(rnum),
            mapFunction(mapFunction),
            keyHashFunction(keyHashFunction),
            reduceFunction(reduceFunction) {};

    void process(const std::string &fileName) {
        auto offsets = calculateOffsets(fileName);
        auto mapResults = doMap(fileName, offsets);
        auto reduceInput = mapToReduce(mapResults);
        doReduce(fileName, reduceInput);
    }


private:
    size_t mnum, rnum;
    const std::function<std::set<std::pair<MapKeyType, MapValueType>>(const InType &)> mapFunction;
    const std::function<size_t(const MapKeyType &)> keyHashFunction;
    const std::function<std::vector<ReduceValueType>(
            const std::vector<std::pair<MapKeyType, MapValueType>> &)> reduceFunction;

    std::set<std::pair<MapKeyType, MapValueType>> mapThread(std::string fileName, size_t from, size_t to) {
        std::ifstream fs(fileName);
        std::set<std::pair<MapKeyType, MapValueType>> results;
        fs.seekg(from);
        size_t len = to - from;
        if (len > 0) {
            std::vector<char> data(len);
            fs.read(data.data(), len);
            auto from = data.begin();
            auto to = data.end();
            while (from != to) {
                auto find = std::find(from, to, '\n');
                std::set<std::pair<MapKeyType, MapValueType>> rowmap = mapFunction(std::string(from, find));
                results.insert(rowmap.begin(), rowmap.end());
                if (find != to) {
                    ++find;
                }
                from = find;
            }
        }
        return results;
    }

    void reduceThread(const std::string &fileName,
                      const std::vector<std::vector<std::pair<MapKeyType, MapValueType>>> &reduceInput,
                      std::mutex &consoleLock, size_t num) const {
        auto reduceResult = reduceFunction(reduceInput[num]);
        std::ofstream os(fileName + '.' + std::__cxx11::to_string(num));
        for (auto &row:reduceResult) {
            os << row << std::endl;
        }
        {
            std::lock_guard<std::mutex> lock(consoleLock);
            std::cout << "Результаты потока " << num << std::endl;
            for (auto &row:reduceResult) {
                std::cout << row << std::endl;
            }
        }
    }

    std::vector<size_t> calculateOffsets(const std::string &fileName) const {
        std::ifstream fs(fileName, std::ios_base::ate);
        if (fs.fail()) {
            throw std::logic_error("Исходный файл не найден");
        }
        size_t totalSize = fs.tellg();
        size_t blockSize = totalSize / mnum;
        std::vector<size_t> offsets(mnum + 1);
        offsets[0] = 0;
        for (size_t block = 0; block < mnum; ++block) {
            size_t newOffset = offsets[block] + blockSize;
            fs.seekg(newOffset, fs.beg);
            if (fs.eof() || fs.fail()) {
                while (block < mnum) {
                    offsets[block + 1] = totalSize + 1;
                    ++block;
                }
                break;
            }
            char c;
            c = fs.get();
            while (c != '\n' && !fs.eof() && !fs.fail()) {
                c = fs.get();
                ++newOffset;
            }
            if (fs.eof() || fs.fail()) {
                while (block < mnum) {
                    offsets[block + 1] = totalSize + 1;
                    ++block;
                }
                break;
            } else {
                offsets[block + 1] = newOffset + 1;
            }
        }
        return offsets;
    }

    std::list<std::set<std::pair<MapKeyType, MapValueType>>> doMap(
            const std::string &fileName,
            const std::vector<size_t> &offsets) {
        std::vector<std::future<std::set<std::pair<MapKeyType, MapValueType>>>> futures;
        futures.reserve(mnum);
        for (size_t blk = 0; blk < mnum; ++blk) {
            futures.push_back(std::async([&, blk]() { return mapThread(fileName, offsets[blk], offsets[blk + 1]); }));
        }

        std::list<std::set<std::pair<MapKeyType, MapValueType>>> mapResults;
        for (auto &future : futures) {
            mapResults.push_back(future.get());
        }
        return mapResults;
    }

    std::vector<std::vector<std::pair<MapKeyType, MapValueType>>>
    mapToReduce(std::list<std::set<std::pair<MapKeyType, MapValueType>>> &mapResults) const {
        std::vector<std::vector<std::pair<MapKeyType, MapValueType>>> reduceInput(rnum);
        while (mapResults.size() > 0) {
            std::optional<std::pair<MapKeyType, MapValueType>> bestPair;
            for (auto &m : mapResults) {
                if (m.begin() != m.end()) {
                    auto rec = *m.begin();
                    if (!bestPair.has_value() || bestPair.value() > rec) {
                        bestPair = rec;
                    }
                }
            }
            for (auto &m : mapResults) {
                if (m.begin() != m.end() && *(m.begin()) == bestPair) {
                    m.erase(m.begin());
                }
            }
            mapResults.remove_if([](auto &rec) { return rec.size() == 0; });
            size_t binNo = keyHashFunction(bestPair.value().first) % rnum;
            reduceInput[binNo].push_back(bestPair.value());
        }
        return reduceInput;
    }

    void doReduce(const std::string &fileName,
                  const std::vector<std::vector<std::pair<MapKeyType, MapValueType>>> &reduceInput) const {
        std::vector<std::future<void>> futures;
        futures.reserve(rnum);
        std::mutex consoleLock;
        for (size_t num = 0; num < rnum; ++num) {
            futures.push_back(std::async([&, num]() {
                reduceThread(fileName, reduceInput, consoleLock, num);
            }));
        }
    }
};