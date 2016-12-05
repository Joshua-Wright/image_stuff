// (c) Copyright 2016 Josh Wright
#pragma once

#include "../types.h"

namespace image_utils {

/**
 *
 * @tparam numeric
 * @param x on range [x0,x1]
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @return
 */
    template<typename numeric>
    numeric linear_interp(numeric x, numeric x0, numeric y0, numeric x1, numeric y1) {
        return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
    }

/**
 *
 * @tparam numeric
 * @param t on range [0,1]
 * @param a
 * @param b
 * @return
 */
    template<typename numeric>
    numeric interp(numeric t, numeric a, numeric b) {
        return a + t * (b - a);
    }

    template<typename numeric>
    numeric clamp(numeric t, numeric a, numeric b) {
        if (t < a) { return a; };
        if (t > b) { return b; };
        return t;
    }

    RGB interp_color(double t, RGB left, RGB right);

};