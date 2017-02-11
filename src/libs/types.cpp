// (c) Copyright 2016 Josh Wright

#include "types.h"

namespace image_utils {

    vec3 RGB_to_vec3(const RGB c) {
        return vec3{
                (double) c.r,
                (double) c.g,
                (double) c.b,
        };
    }

    RGB vec3_to_RGB(const vec3 &v) {
        return RGB{
                clamp<unsigned char, double>(v[0], 0, 255),
                clamp<unsigned char, double>(v[1], 0, 255),
                clamp<unsigned char, double>(v[2], 0, 255),
        };
    }
}