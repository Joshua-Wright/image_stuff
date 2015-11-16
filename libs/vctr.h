//
// Created by j0sh on 11/13/15.
//

#ifndef IMAGE_STUFF_VECTOR_H
#define IMAGE_STUFF_VECTOR_H

namespace image_utils {

    class vctr {
    public:
        long double x;
        long double y;

        vctr(long double _x, long double _y);

        vctr operator*(long double rhs) const;

        vctr operator/(long double rhs) const;

        vctr operator+(vctr rhs) const;

        vctr operator-(vctr rhs) const;

        vctr &operator*=(long double rhs);

        vctr &operator/=(long double rhs);

        vctr &operator+=(vctr rhs);

        vctr &operator-=(vctr rhs);

        long double mag();

        long double mag2();

    };
}
#endif //IMAGE_STUFF_VECTOR_H
