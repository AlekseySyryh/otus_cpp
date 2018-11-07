#pragma once

#include <map>
#include <memory>
#include <numeric>

template<class T, T def, size_t dims = 2>
class Matrix {
public:
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
        }
    }

    Matrix<T, def, dims - 1> &operator[](size_t ix) {
        return data[ix];
    }

private:
    std::map<size_t, Matrix<T, def, dims - 1>> data;
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

private:
    Matrix<T, def, 1> &parent;
    size_t ix;
};


template<class T, T def>
class Matrix<T, def, 1> {
public:
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
        }
    }

    T get(size_t ix) {
        if (data.count(ix) != 0) {
            return data[ix];
        } else {
            return def;
        }
    }

    auto begin() const {
        return data.cbegin();
    }

    auto end() const {
        return data.cend();
    }
private:
    std::map<size_t, T> data;
};

