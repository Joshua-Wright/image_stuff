//
// Created by j0sh on 4/5/16.
//

#ifndef CONTAINERS_VECT_H
#define CONTAINERS_VECT_H

#include <cmath>
#include <array>
#include <algorithm>
#include <numeric> // for accumulate

namespace containers {


  template<typename T, size_t dim>
  struct vect : public std::array<T, dim> {
    typedef std::array<T, dim> array;
    using array::begin;
    using array::end;
    using array::cbegin;
    using array::cend;

  public:

    vect() : array() { }

    vect(const T &val) : array() { std::fill(begin(), end(), val); }

    template<typename U>
    vect(const vect<U, dim> &rhs) {
      std::copy(rhs.begin(), rhs.end(), begin());
    }

    vect(const T* data) {
      std::copy_n(data, dim, begin());
    }

    vect(const std::initializer_list<T> &list) : array() {
      if (list.size() != dim) {
        throw std::logic_error("Bad array initializer size!");
      }
      std::copy(list.begin(), list.end(), begin());
    }


    /*scalars*/
    vect<T, dim> &operator+=(const T &rhs) {
      std::transform(begin(), end(), begin(),
                     [&rhs](const T &a) { return a + rhs; });
      return *this;
    };

    vect<T, dim> &operator-=(const T &rhs) {
      std::transform(begin(), end(), begin(),
                     [&rhs](const T &a) { return a - rhs; });
      return *this;
    };

    vect<T, dim> &operator*=(const T &rhs) {
      std::transform(begin(), end(), begin(),
                     [&rhs](const T &a) { return a * rhs; });
      return *this;
    };

    vect<T, dim> &operator/=(const T &rhs) {
      std::transform(begin(), end(), begin(),
                     [&rhs](const T &a) { return a / rhs; });
      return *this;
    };

    vect<T, dim> &operator%=(const T &rhs) {
      std::transform(begin(), end(), begin(),
                     [&rhs](const T &a) { return a % rhs; });
      return *this;
    };

    /*vectors*/
    vect<T, dim> operator+=(const vect<T, dim> &rhs) {
      for (auto i1 = begin(), i2 = rhs.begin(); i1 < end(); ++i1, ++i2) {
        (*i1) += (*i2);
      }
      return *this;
    }

    vect<T, dim> operator-=(const vect<T, dim> &rhs) {
      for (auto i1 = begin(), i2 = rhs.begin(); i1 < end(); ++i1, ++i2) {
        *i1 -= *i2;
      }
      return *this;
    }

    vect<T, dim> operator*=(const vect<T, dim> &rhs) {
      for (auto i1 = begin(), i2 = rhs.begin(); i1 < end(); ++i1, ++i2) {
        *i1 *= *i2;
      }
      return *this;
    }

    vect<T, dim> operator/=(const vect<T, dim> &rhs) {
      for (auto i1 = begin(), i2 = rhs.begin(); i1 < end(); ++i1, ++i2) {
        *i1 /= *i2;
      }
      return *this;
    }

    vect<T, dim> operator%=(const vect<T, dim> &rhs) {
      for (auto i1 = begin(), i2 = rhs.begin(); i1 < end(); ++i1, ++i2) {
        *i1 %= *i2;
      }
      return *this;
    }

    vect<T, dim> operator+(const vect<T, dim> &rhs) const {
      vect<T, dim> out(*this);
      out += rhs;
      return out;
    }

    vect<T, dim> operator-(const vect<T, dim> &rhs) const {
      vect<T, dim> out(*this);
      out -= rhs;
      return out;
    }

    vect<T, dim> operator*(const vect<T, dim> &rhs) const {
      vect<T, dim> out(*this);
      out *= rhs;
      return out;
    }

    vect<T, dim> operator/(const vect<T, dim> &rhs) const {
      vect<T, dim> out(*this);
      out /= rhs;
      return out;
    }

    vect<T, dim> operator%(const vect<T, dim> &rhs) const {
      vect<T, dim> out(*this);
      out %= rhs;
      return out;
    }


    vect<T, dim> pow(const T &rhs) const {
      vect<T, dim> out;
      std::transform(begin(), end(), out.begin(),
                     [&rhs](const T &a) { return std::pow(a, rhs); });
      return out;
    };

    T norm2() const {
      return std::accumulate(begin(), end(), T(), [](const T &cur, const T &b) {
        return cur + b * b;
      });
    };

    T norm() const { return std::sqrt(norm2()); }

    vect<T, dim> unitV() const { return (*this) / norm(); };

    T sum() const {
      return std::accumulate(begin(), end(), T(), [](const T &cur, const T &b) {
        return cur + b;
      });
    }

    T avg() const {
      return sum() / dim;
    };

    T dist2(const vect<T, dim> &rhs) const { return (rhs - *this).norm2(); }

    T dist(const vect<T, dim> &rhs) const { return std::sqrt(dist2(rhs)); }

    template<typename U>
    operator vect<U, dim>() const {
      vect<U, dim> out = *this;
      return out;
    };

  };

  template<typename T, size_t dim>
  vect<T, dim> operator+(const vect<T, dim> &lhs, const T &rhs) {
    vect<T, dim> out = lhs;
    out += rhs;
    return out;
  }

  template<typename T, size_t dim>
  vect<T, dim> operator-(const vect<T, dim> &lhs, const T &rhs) {
    vect<T, dim> out = lhs;
    out -= rhs;
    return out;
  }

  template<typename T, size_t dim>
  vect<T, dim> operator*(const vect<T, dim> &lhs, const T &rhs) {
    vect<T, dim> out = lhs;
    out *= rhs;
    return out;
  }

  template<typename T, size_t dim>
  vect<T, dim> operator/(const vect<T, dim> &lhs, const T &rhs) {
    vect<T, dim> out = lhs;
    out /= rhs;
    return out;
  }

  template<typename T, size_t dim>
  vect<T, dim> operator%(const vect<T, dim> &lhs, const T &rhs) {
    vect<T, dim> out = lhs;
    out %= rhs;
    return out;
  }

  template<typename T, size_t dim>
  vect<T, dim> operator+(const T &rhs, const vect<T, dim> &lhs) {
    vect<T, dim> out = lhs;
    out += rhs;
    return out;
  }

  template<typename T, size_t dim>
  vect<T, dim> operator-(const T &rhs, const vect<T, dim> &lhs) {
    vect<T, dim> out;
    std::transform(lhs.cbegin(), lhs.cend(), out.begin(),
                   [&rhs](const T &a) { return rhs - a; });
    return out;
  }

  template<typename T, size_t dim>
  vect<T, dim> operator*(const T &rhs, const vect<T, dim> &lhs) {
    vect<T, dim> out = lhs;
    out *= rhs;
    return out;
  }

  template<typename T, size_t dim>
  vect<T, dim> operator/(const T &rhs, const vect<T, dim> &lhs) {
    vect<T, dim> out;
    std::transform(lhs.begin(), lhs.end(), out.begin(),
                   [&rhs](const T &a) { return rhs / a; });
    return out;
  }

  template<typename T>
  vect<T, 3> crossP(const vect<T, 3> &u, const vect<T, 3> &v) {
    vect<T, 3> out;
    out[0] = u[1] * v[2] - u[2] * v[1];
    out[1] = u[2] * v[0] - u[0] * v[2];
    out[2] = u[0] * v[1] - u[1] * v[0];
    return out;
  };

  template<typename T>
  vect<T, 3> crossP(const vect<T, 2> &u, const vect<T, 2> &v) {
    vect<T, 3> out;
    out[0] = 0;
    out[1] = 0;
    out[2] = u[0] * v[1] - u[1] * v[0];
    return out;
  };

}


#endif //CONTAINERS_VECT_H
