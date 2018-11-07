#pragma once

#include <map>
#include <memory>

template<class T, T def, size_t dims = 2>
class Matrix {
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
    size_t size() {
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

