#pragma once

template<typename T>
class MatrixIterator {
public:
    MatrixIterator() = default;

    MatrixIterator(T current, T end) : current(current), end(end) {
        if (current != end) {
            child = current->second.begin();
        }
    }

    void operator++() {
        if (child != current->second.end()) {
            ++child;
        }
        if (child == current->second.end()) {
            ++current;
            if (current != end) {
                child = current->second.begin();
            }
        }
    }

    MatrixIterator<T> &operator=(const MatrixIterator<T> &other) {
        current = other.current;
        child = other.child;
        end = other.end;
        return *this;
    }

    auto operator*() const {
        return std::tuple_cat(
                std::make_tuple(current->first),
                *child);
    }

    bool operator==(const MatrixIterator<T> &rhs) const {
        return (current == rhs.current) &&
               ((child == rhs.child) || (current == end));
    }

    bool operator!=(const MatrixIterator<T> &rhs) const {
        return !(*this == rhs);
    }

private:
    T current;
    decltype(current->second.begin()) child;
    T end;
};
