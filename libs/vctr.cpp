//
// Created by j0sh on 11/13/15.
//

#include <cmath>
#include "vctr.h"

namespace image_utils {

    vctr::vctr(long double _x, long double _y) : x(_x), y(_y) { }

    vctr vctr::operator*(long double rhs) const {
        return vctr(x * rhs, y * rhs);
    }

    vctr vctr::operator/(long double rhs) const {
        return vctr(x / rhs, y / rhs);
    }

    vctr vctr::operator+(vctr rhs) const {
        return vctr(x + rhs.x, y + rhs.y);
    }

    vctr vctr::operator-(vctr rhs) const {
        return vctr(x - rhs.x, y - rhs.y);
    }

    vctr &vctr::operator*=(long double rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    vctr &vctr::operator/=(long double rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    vctr &vctr::operator+=(vctr rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    vctr &vctr::operator-=(vctr rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    long double vctr::mag() {
        return std::sqrt(x * x + y * y);
    }

    long double vctr::mag2() {
        return x * x + y * y;
    }

}