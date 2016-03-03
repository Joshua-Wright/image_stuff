// (c) Copyright 2015 Josh Wright
#pragma once

#include "types.h"

namespace image_utils {

    //////////////////
    // colormap ADT //
    //////////////////
    /*colormaps expect 0 <= x <= 1 */
    class colormap {
    public:
        virtual RGB get_rgb(const long double x) const = 0;
    };
    class colormap_basic_hot : public colormap {
    public:
        virtual RGB get_rgb(const long double d) const;
    };
    class colormap_grayscale : public colormap {

    public:
        virtual RGB get_rgb(const long double x) const;
    };



    void grayscale_to_rgb(const matrix<long double> &in_double, image_RGB &out_rgb,
                          colormap *fun);


}