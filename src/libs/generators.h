// (c) Copyright 2015 Josh Wright
#pragma once

#include "types.h"

namespace image_utils {

    ///////////
    // waves //
    ///////////
    /*
     * void* d: pointer to wave specific parameters or config data
     * long double x: x-value in graph
     * all normalized to  0 <= output <= 1 on domain 0 <= x <= 1
     */
    typedef long double (*wave_function)(void *, long double);

    long double fourier_square_wave(long double x, size_t n);

    long double square_wave(void*, long double x);

    long double sine_wave(void *, long double x);

    long double triangle_wave(void *, long double x);

    ///////////////////////



    /////////////
    // fillers //
    /////////////
    void image_fill_circle_grid(matrix<long double> &grid,
                                long double theta_mul, long double dist_mul,
                                wave_function func = sine_wave,
                                void *wave_param = NULL);


    ///////////////////////

}
