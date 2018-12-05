#pragma once

template<typename T, size_t dims>
class MatrixIterator {
public:
    MatrixIterator(typename std::map<std::array<size_t, dims>, T>::iterator iterator) : iterator(iterator) {

    }

    MatrixIterator(std::map<std::array<size_t, dims>, T> &container) {
        iterator = container.begin();
    }

    bool operator!=(const MatrixIterator<T, dims> other) {
        return iterator != other.iterator;
    }

    void operator++() {
        ++iterator;
    }

    auto operator*() {
        return std::tuple_cat(
                std::make_tuple(iterator->first[0]),
                std::make_tuple(iterator->second));
    }

private:
    typename std::map<std::array<size_t, dims>, T>::iterator iterator;
};