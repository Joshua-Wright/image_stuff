//
// Created by j0sh on 11/15/15.
//

#ifndef IMAGE_STUFF_TYPES_H
#define IMAGE_STUFF_TYPES_H

#include <cstring>
#include <complex>
#include "util/matrix.h"
#include "util/vect.h"


namespace image_utils {

    using containers::matrix;
    using containers::assert_same_size;

    using std::sqrt;
    using std::sin;
    using std::cos;
    using std::pow;
    using std::log;
    using std::log2;
    using std::norm;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
    /*keep these constants here for conveniece even if they aren't used*/
    const double PI = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482;
    const double INF = std::numeric_limits<double>::infinity();
    const double SQRT_EPSILON = std::sqrt(std::numeric_limits<double>::epsilon());
#pragma clang diagnostic pop

    typedef containers::vect<double, 2> vec2;
    typedef containers::vect<double, 3> vec3;
    typedef containers::vect<double, 4> vec4;
    typedef containers::vect<size_t, 2> vec_ull;
    typedef containers::vect<long, 2> vec_ll;

    struct grayscale {
        unsigned char g;
    };

    struct RGB {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    struct RGBA : RGB {
        unsigned char a;
    };

    struct grayscaled : grayscale { // d for defined
        bool d;
    };

    struct RGBd : RGB { // d for defined
        bool d;
    };

    struct RGBAd : RGBA { // d for defined
        bool d;
    };

    template<typename Iter1, typename Iter2>
    bool memcmp_iter_equal(const Iter1 it1, const Iter2 it2) {
        if (sizeof(decltype(*it1)) != sizeof(decltype(*it2))) {
            /*different sizes can't be equal*/
            return false;
        }
        return std::memcmp(&(*it1), &(it2), sizeof(decltype(*it1)));
    };

    template<typename T1, typename T2>
    bool memcmp_equal(const T1 &a, const T2 &b) {
        if (sizeof(T1) != sizeof(T2)) {
            /*different sizes can't be equal*/
            return false;
        }
        return std::memcmp(&a, &b, sizeof(a));
    };

    typedef matrix<grayscale> image_gs;
    typedef matrix<RGB> image_RGB;
    typedef matrix<RGBA> image_RGBA;
    typedef matrix<RGBd> image_RGBd;
    typedef matrix<RGBAd> image_RGBAd;
    typedef matrix<grayscaled> image_gsd;

    typedef std::complex<double> complex;

}
#endif //IMAGE_STUFF_TYPES_H
