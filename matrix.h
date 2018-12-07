#pragma once

#include <map>
#include <memory>
#include <numeric>
#include <tuple>
#include <array>
#include <ostream>
#include <vector>
#include <algorithm>
#include "matrix_iterator.h"

template<typename T, T def, size_t dims>
class DataStorage {
public:
    virtual T get(std::array<size_t, dims>) const { return def; };

    virtual void set(std::array<size_t, dims>, T) {};
};

template<class T, T def, size_t dims, size_t indexLen>
class SubMatrix {
public:
    SubMatrix(DataStorage<T, def, dims> *dataStorage,
              std::array<size_t, indexLen> ix) : ds(dataStorage), ix(ix) {}

    SubMatrix<T, def, dims, indexLen + 1> operator[](size_t lastIx) {
        std::array<size_t, indexLen + 1> newIx;
        std::copy(ix.begin(), ix.end(), newIx.begin());
        newIx[indexLen] = lastIx;
        return SubMatrix<T, def, dims, indexLen + 1>(ds, newIx);
    }

private:
    DataStorage<T, def, dims> *ds;
    std::array<size_t, indexLen> ix;
};

template<class T, T def, size_t dims>
class SubMatrix<T, def, dims, dims> {
public:
    SubMatrix(DataStorage<T, def, dims> *dataStorage,
              std::array<size_t, dims> ix) : ds(dataStorage), ix(ix) {}

    bool operator==(const T &rhs) {
        return ds->get(ix) == rhs;
    }

    SubMatrix<T, def, dims, dims> operator=(const T &newValue) {
        ds->set(ix, newValue);
        return SubMatrix<T, def, dims, dims>(ds, ix);
    }

    T get() const {
        return ds->get(ix);
    }

private:
    DataStorage<T, def, dims> *ds;
    std::array<size_t, dims> ix;
};

template<class T, T def, size_t dims>
std::ostream &operator<<(std::ostream &os, const SubMatrix<T, def, dims, dims> &subMatrix) {
    os << subMatrix.get();
    return os;
}

template<class T, T def, size_t dims = 2>
class Matrix : public DataStorage<T, def, dims> {
public:
    size_t size() {
        return data.size();
    }

    T get(std::array<size_t, dims> addr) const override {
        auto it = data.find(addr);
        if (it != data.end()) {
            return it->second;
        } else {
            return def;
        }
    }

    void set(std::array<size_t, dims> addr, T val) override {
        if (val == def) {
            data.erase(addr);
        } else {
            data[addr] = val;
        }
    }

    SubMatrix<T, def, dims, 1> operator[](size_t ix) {
        return SubMatrix<T, def, dims, 1>(this, {ix});
    }

    auto begin() {
        return MatrixIterator<T, dims>(data);
    }

    auto end() {
        return MatrixIterator<T, dims>(data.end());
    }

private:
    std::map<std::array<size_t, dims>, T> data;
};


