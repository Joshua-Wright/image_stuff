// (c) Copyright 2016 Josh Wright

#include "linear_interp.h"

namespace image_utils {

    RGB interp_color(double t, RGB left, RGB right) {
        return RGB{
                (unsigned char) clamp(interp<double>(t, left.r, right.r), 0.0, 255.0),
                (unsigned char) clamp(interp<double>(t, left.g, right.g), 0.0, 255.0),
                (unsigned char) clamp(interp<double>(t, left.b, right.b), 0.0, 255.0),
        };
    }
};