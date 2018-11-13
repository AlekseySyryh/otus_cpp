#pragma once

#include <map>
#include <memory>
#include <numeric>
#include <tuple>
#include <ostream>
#include <vector>
#include "matrix_iterator.h"

struct IMatrix {
    virtual void delRow(size_t) = 0;
};

template<class T, T def, size_t dims = 2>
class Matrix : IMatrix {
public:
    Matrix() : parent(nullptr) {}

    Matrix(IMatrix *parent, size_t ix) : parent(parent), ix(ix) {

    }

    size_t size() const {
        return std::accumulate(
                data.begin(),
                data.end(),
                0,
                [](const auto &lhs, const auto &rhs) {
                    return rhs.second.size() + lhs;
                });
    }

    template<typename ...Args>
    T get(size_t dim, Args... args) {
        static_assert(sizeof...(Args) == dims - 1, "Wrong number of params");
        if (data.count(dim) == 1) {
            return data[dim].get(args...);
        } else {
            return def;
        }
    }

    template<typename ...Args>
    void set(size_t dim, Args... args) {
        static_assert(sizeof...(Args) == dims, "Wrong number of params");
        data[dim].set(args...);
        if (data[dim].size() == 0) {
            data.erase(dim);
            deleteIfNoLongerRequired();
        }
    }

    Matrix<T, def, dims - 1> &operator[](size_t ix) {
        if (data.count(ix) == 0) {
            data[ix] = Matrix<T, def, dims - 1>(this, ix);
        }
        return data[ix];
    }

    using iterator = MatrixIterator<typename std::map<size_t, Matrix<T, def, dims - 1>>::iterator>;

    iterator begin() {
        return iterator(data.begin(), data.end());
    }

    iterator end() {
        return iterator(data.end(), data.end());
    }

    void delRow(size_t ix) {
        data.erase(ix);
        deleteIfNoLongerRequired();
    }
private:
    std::map<size_t, Matrix<T, def, dims - 1>> data;
    IMatrix *parent;
    size_t ix;

    void deleteIfNoLongerRequired() {
        if (data.size() == 0 &&
            parent != nullptr) {
            parent->delRow(ix);
        }
    }
};

template<class T, T def>
struct MatrixRec {
    MatrixRec(Matrix<T, def, 1> &parent, size_t ix) : parent(parent), ix(ix) {

    }

    bool operator==(const T &rhs) {
        return parent.get(ix) == rhs;
    }

    MatrixRec &operator=(const T &rhs) {
        parent.set(ix, rhs);
        return *this;
    }

    T get() {
        return parent.get(ix);
    }
private:
    Matrix<T, def, 1> &parent;
    size_t ix;
};

template<class T, T def>
std::ostream &operator<<(std::ostream &str, MatrixRec<T, def> &&rec) {
    str << rec.get();
    return str;
}


template<class T, T def>
class Matrix<T, def, 1> {
public:
    Matrix() : parent(nullptr) {}

    Matrix(IMatrix *parent, size_t ix) : parent(parent), ix(ix) {

    }
    size_t size() const {
        return data.size();
    }

    auto operator[](size_t ix) {
        return MatrixRec<T, def>(*this, ix);
    }

    void set(size_t ix, const T &val) {
        if (val != def) {
            data[ix] = val;
        } else {
            data.erase(ix);
            if (data.size() == 0 &&
                parent != nullptr) {
                parent->delRow(ix);
            }
        }
    }

    T get(size_t ix) {
        if (data.count(ix) != 0) {
            return data[ix];
        } else {
            return def;
        }
    }

    using iterator = typename std::map<size_t, T>::iterator;

    iterator begin() {
        return data.begin();
    }

    iterator end() {
        return data.end();
    }
private:
    std::map<size_t, T> data;
    IMatrix *parent;
    size_t ix;
};

