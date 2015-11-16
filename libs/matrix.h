#pragma once

#include <cstring>
#include <type_traits>
#include <stdexcept>

namespace image_utils {

    template<typename T>
    class matrix {
        unsigned width, height;
        T *data;

        /* there's a specialization of this in matrix.cpp */
        void initialize_zero() {
            std::memset(data, width * height, sizeof(T));
        }

    public:
        matrix(unsigned x, unsigned y) : width(x), height(y) {
            data = new T[width * height];
            initialize_zero();
        };

        unsigned x() const {
            return width;
        }

        unsigned y() const {
            return height;
        }

        unsigned size() const {
            return width * height;
        }

        T *begin() { // first element
            return data;
        }

        T *end() { // past the end pointer
            return data + ((height - 1) * width + height);
        }

        const T *begin() const { // first element
            return (T *) data;
        }

        const T *end() const { // past the end pointer
            return data + ((height - 1) * width + height);
        }

        T &operator()(unsigned x, unsigned y) {
            if (x >= width || y >= height) {
                throw std::out_of_range("out of range");
            }
            return data[x * width + y];
        }

        const T &operator()(unsigned x, unsigned y) const {
            if (x >= width || y >= height) {
                throw std::out_of_range("out of range");
            }

            return data[x * width + y];
        }

        T &operator()(unsigned z) {
            if (z >= (width * height)) {
                throw std::out_of_range("out of range");
            }
            return data[z];
        }

        const T &operator()(unsigned z) const {
            if (z >= (width * height)) {
                throw std::out_of_range("out of range");
            }
            return data[z];
        }

        template<typename R>
        matrix<T> &operator/=(const R rhs) {
            static_assert(std::is_arithmetic<R>::value, "R must be an arithmetic type!");
            for (auto &x : *this) {
                x /= rhs;
            }
        }

        template<typename R>
        matrix<T> &operator*=(const R rhs) {
            static_assert(std::is_arithmetic<R>::value, "R must be an arithmetic type!");
            for (auto &x : *this) {
                x *= rhs;
            }
        }

        template<typename R>
        matrix<T> &operator+=(const R rhs) {
            static_assert(std::is_arithmetic<R>::value, "R must be an arithmetic type!");
            for (auto &x : *this) {
                x += rhs;
            }
        }

        template<typename R>
        matrix<T> &operator-=(const R rhs) {
            static_assert(std::is_arithmetic<R>::value, "R must be an arithmetic type!");
            for (auto &x : *this) {
                x -= rhs;
            }
        }

    };

    template<typename T, typename F>
    bool dimensions_equal(matrix<T> a, matrix<F> b) {
        return (a.x() == b.x()) && (a.y() == b.y());
    };

    template<typename T, typename F>
    void assert_same_size(matrix<T> a, matrix<F> b) {
        if (!(a.x() == b.x()) && (a.y() == b.y())) {
            throw std::runtime_error("Dimensions must match!");
        }
    };

}