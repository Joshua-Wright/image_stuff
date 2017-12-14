// (c) Copyright 2015 Josh Wright
#include "expand_pixels.h"

namespace image_utils {

    matrix<RGB> expand_pixels(const matrix<RGB> &in, const size_t px_size) {
        matrix<RGB> out(in.x() * px_size, in.y() * px_size);
#pragma omp parallel for schedule(static) collapse(2)
        for (size_t x = 0; x < out.x(); ++x) {
            for (size_t y = 0; y < out.y(); ++y) {

                size_t ix = x / px_size;
                size_t iy = y / px_size;

                if ((x % px_size) < px_size / 3) {
                    out(x, y).r = in(ix, iy).r;
                } else if ((x % px_size) < 2 * px_size / 3) {
                    out(x, y).g = in(ix, iy).g;
                } else {
                    out(x, y).b = in(ix, iy).b;
                }
            }
        }
        return out;
    }

}