// (c) Copyright 2015 Josh Wright
#pragma once

#include "types.h"

namespace image_utils {

    /*TODO: ADT-based colormaps, similar to wave setup*/

    typedef RGB (*colormap_f)(long double);

    RGB colormap_basic_hot(long double x);

    RGB colormap_grayscale(long double x);


    void grayscale_to_rgb(matrix<long double> &in_double, image_RGB &out_rgb,
                          colormap_f fun);

}