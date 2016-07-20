#ifndef CONTAINERS_MATRIX
#define CONTAINERS_MATRIX

#include <stdexcept>
#include <type_traits>
#include <algorithm>
#include "vect.h"

namespace containers {

    template<typename T>
    class matrix;

    template<typename T, typename U>
    bool dimensions_equal(const matrix<T> &a, const matrix<U> &b);

    template<typename T, typename U>
    void assert_same_size(const matrix<T> &a, const matrix<U> &b);

    template<typename T>
    class matrix {
        std::size_t width, height;
        T *_data;

    public:

        typedef void *(*func_alloc)(size_t);

        typedef void (*func_free)(void *);

        func_alloc alloc;
        func_free dealloc;

        matrix(const std::size_t x, const std::size_t y,
               const T initial_value = T()) : width(x), height(y),
                                              _data(new T[width * height]),
                                              alloc(nullptr), dealloc(nullptr) {
            std::fill_n(_data, width * height, initial_value);
        };

        matrix(const std::size_t x, const std::size_t y,
               func_alloc allocator,
               func_free deallocator) : width(x), height(y),
                                        alloc(allocator), dealloc(deallocator) {
            _data = (T *) alloc(width * height * sizeof(T));
            std::fill_n(_data, width * height, T());
        }

        matrix(const matrix<T> &lhs) : width(lhs.width), height(lhs.height),
                                       _data(new T[width * height]) {
            std::copy_n(lhs._data, width * height, _data);
        }


        ~matrix() {
//      if (dealloc == nullptr) {
//        delete[] _data;
//      } else {
//        dealloc(_data);
//      }
        }

        matrix &operator=(const matrix<T> &lhs) {
            if (&lhs == this) {
                /*we are being assigned to ourself*/
                return *this;
            }
            if (dealloc == nullptr) {
                delete[] _data;
            } else {
                dealloc(_data);
            }
            width = lhs.width;
            height = lhs.height;
            _data = new T[width * height];
            std::copy_n(lhs._data, width * height, _data);
            return *this;
        }

        matrix(matrix<T> &&lhs) : width(lhs.width), height(lhs.height),
                                  _data(lhs._data) {
            lhs._data = nullptr;
            lhs.width = 0;
            lhs.height = 0;
        }

        matrix<T> &operator=(matrix<T> &&lhs) {
            if (&lhs == this) {
                /*we are being moved to ourself*/
                return *this;
            }
            delete[] _data;
            width = lhs.width;
            height = lhs.height;
            _data = lhs._data;
            lhs._data = nullptr;
            lhs.width = 0;
            lhs.height = 0;
            return *this;
        }

        template<typename U>
        void fill(const U &val) { std::fill(_data, _data + (width * height), val); }

        std::size_t x() const {
            return width;
        }

        std::size_t y() const {
            return height;
        }

        std::size_t size() const {
            return width * height;
        }

        T *data() {
            /*simple getter for the underlying array*/
            return _data;
        }

        const T *data() const {
            /*simple getter for the underlying array*/
            return _data;
        }

        T *begin() { // first element
            return _data;
        }

        T *end() { // past the end pointer
            return _data + height * width;
        }

        const T *begin() const { // first element
            return (T *) _data;
        }

        const T *end() const { // past the end pointer
            return _data + height * width;
        }

        const T *cbegin() const { // first element
            return (T *) _data;
        }

        const T *cend() const { // past the end pointer
            return _data + height * width;
        }

        T &operator()(const std::size_t x, const std::size_t y) {
            if (x >= width || y >= height) {
                throw std::out_of_range("out of range");
            }
            return _data[y * width + x];
        }

        const T &operator()(const std::size_t x, const std::size_t y) const {
            if (x >= width || y >= height) {
                throw std::out_of_range("out of range");
            }
            return _data[y * width + x];
        }

        T &operator()(const vect<size_t, 2> &p) {
            return (*this)(p[0], p[1]);
        }

        const T &operator()(const vect<size_t, 2> &p) const {
            return (*this)(p[0], p[1]);
        }

        T &operator()(std::size_t z) {
            if (z >= (width * height)) {
                throw std::out_of_range("out of range");
            }
            return _data[z];
        }

        const T &operator()(std::size_t z) const {
            if (z >= (width * height)) {
                throw std::out_of_range("out of range");
            }
            return _data[z];
        }

        std::size_t z_to_x(std::size_t z) { return z / height; };

        std::size_t z_to_y(std::size_t z) { return z % height; };


        template<typename F>
        matrix<T> &operator*=(const F &lhs) {
            std::transform(begin(), end(), begin(),
                           [&lhs](const T &n) { return n * lhs; });
            return *this;
        };

        template<typename F>
        matrix<T> &operator/=(const F &lhs) {
            std::transform(begin(), end(), begin(),
                           [&lhs](const T &n) { return n / lhs; });
            return *this;
        };

        template<typename F>
        matrix<T> &operator+=(const F &lhs) {
            std::transform(begin(), end(), begin(),
                           [&lhs](const T &n) { return n + lhs; });
            return *this;
        };

        template<typename F>
        matrix<T> &operator-=(const F &lhs) {
            std::transform(begin(), end(), begin(),
                           [&lhs](const T &n) { return n - lhs; });
            return *this;
        };

        template<typename F>
        matrix<T> &operator%=(const F &lhs) {
            std::transform(begin(), end(), begin(),
                           [&lhs](const T &n) { return n % lhs; });
            return *this;
        };

        template<typename F>
        matrix<T> &fmod_in_place(const F &rhs) {
            std::transform(begin(), end(), begin(),
                           [&rhs](const T &n) { return std::fmod(n, rhs); });
            return *this;
        }

        template<typename U>
        matrix<T> &operator*=(const matrix<U> &lhs) {
            assert_same_size(*this, lhs);
            std::transform(begin(), end(), lhs.begin(), begin(),
                           [](const T &a, const U &b) { return a * b; });
            return *this;
        };

        template<typename U>
        matrix<T> &operator/=(const matrix<U> &lhs) {
            assert_same_size(*this, lhs);
            std::transform(begin(), end(), lhs.begin(), begin(),
                           [](const T &a, const U &b) { return a / b; });
            return *this;
        };

        template<typename U>
        matrix<T> &operator+=(const matrix<U> &lhs) {
            assert_same_size(*this, lhs);
            std::transform(begin(), end(), lhs.begin(), begin(),
                           [](const T &a, const U &b) { return a + b; });
            return *this;
        };

        template<typename U>
        matrix<T> &operator-=(const matrix<U> &lhs) {
            assert_same_size(*this, lhs);
            std::transform(begin(), end(), lhs.begin(), begin(),
                           [](const T &a, const U &b) { return a - b; });
            return *this;
        };

        template<typename U>
        matrix<T> &operator%=(const matrix<U> &lhs) {
            assert_same_size(*this, lhs);
            std::transform(begin(), end(), lhs.begin(), begin(),
                           [](const T &a, const U &b) { return a % b; });
            return *this;
        };

        template<typename U>
        bool operator==(const matrix<U> &lhs) {
            if (!dimensions_equal(*this, lhs)) {
                return false;
            }
            return std::equal(begin(), end(), lhs.begin());
        }

        template<typename U>
        bool operator!=(const matrix<U> &lhs) {
            return !((*this) == lhs);
        }

    };

    template<typename T, typename U>
    bool dimensions_equal(const matrix<T> &a, const matrix<U> &b) {
        return (a.x() == b.x()) && (a.y() == b.y());
    };

    template<typename T, typename U>
    void assert_same_size(const matrix<T> &a, const matrix<U> &b) {
        if (!dimensions_equal(a, b)) {
            throw std::invalid_argument("Dimensions must match!");
        }
    };

}

#endif /*CONTAINERS_MATRIX*/