//
// Created by j0sh on 5/27/16.
//

#ifndef IMAGE_STUFF_MANDELBROT_H
#define IMAGE_STUFF_MANDELBROT_H

#include "types.h"

namespace image_utils {
    /*bounds is in the form {xmin, xmax, ymin, ymax} */
    void mandelbrot(size_t iterations, matrix<double> &grid,
                    std::array<double, 4> bounds = {-2, 2, -2, 2});
}
#endif //IMAGE_STUFF_MANDELBROT_H
