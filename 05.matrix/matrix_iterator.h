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
        return
                std::tuple_cat(
                        make_tuple_from_array(iterator->first),
                        std::make_tuple(iterator->second));
    }

private:
    typename std::map<std::array<size_t, dims>, T>::iterator iterator;

    template<size_t len>
    auto make_tuple_from_array(const std::array<size_t, len> &array) {
        auto it = array.begin();
        ++it;
        std::array<size_t, len - 1> subarray;
        std::copy(it, array.end(), subarray.begin());
        return std::tuple_cat(
                std::make_tuple(array[0]),
                make_tuple_from_array(subarray));
    }

    auto make_tuple_from_array(const std::array<size_t, 1> &array) {
        return std::make_tuple(array[0]);
    }
};