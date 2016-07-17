//
// Created by j0sh on 5/27/16.
//

#ifndef IMAGE_STUFF_MANDELBROT_H
#define IMAGE_STUFF_MANDELBROT_H

#include "types.h"

namespace image_utils {
    /*bounds is in the form {xmin, xmax, ymin, ymax} */

    matrix<double> fast_mandelbrot(const size_t w, const size_t h, const size_t iter, const std::array<double, 4> bounds);

    matrix<double> fast_julia(const size_t w, const size_t h, const size_t iter, const std::array<double, 4> bounds, complex c);
}
#endif //IMAGE_STUFF_MANDELBROT_H
