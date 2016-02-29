// (c) Copyright 2015 Josh Wright
#pragma once

#ifndef IMAGE_UTILS_GENERATORS
#define IMAGE_UTILS_GENERATORS

#include "types.h"

namespace image_utils {

    /////////////////////
    // wave generators //
    /////////////////////
    class wave {
    public:
        virtual long double operator()(const long double &x) const = 0;
    };
    class wave_triangle : public wave {
    public:
        virtual long double operator()(const long double &x) const;
    };
    class wave_sine : public wave {
    public:
        virtual long double operator()(const long double &x) const;
    };
    class wave_square : public wave {
    public:
        virtual long double operator()(const long double &x) const;
    };
    class wave_fourier_square : public wave {
        size_t n;
    public:
        wave_fourier_square(size_t _n) : n(_n) {}
        virtual long double operator()(const long double &x) const;
    };


    /////////////
    // fillers //
    /////////////

    /*
     * theta_mul: larger => more angular ripples (each ripple is smaller)
     * dist_mult: larger => more radial ripples (each ripple is smaller)
     */
    void image_fill_circle_grid(matrix<long double> &grid,
                                long double theta_mul, long double dist_mul,
                                wave *wave_func = nullptr);


}


#endif /*IMAGE_UTILS_GENERATORS*/