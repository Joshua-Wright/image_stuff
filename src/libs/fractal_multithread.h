//
// Created by j0sh on 5/27/16.
//

#ifndef IMAGE_STUFF_MANDELBROT_H
#define IMAGE_STUFF_MANDELBROT_H

#include <map>
#include "types.h"
#include "fractal_common.h"

namespace image_utils {

    class fractal_multithread : public fractal_base {
    public:
        fractal_multithread(const size_t w, const size_t h);
        matrix<double> run();
    };
}
#endif //IMAGE_STUFF_MANDELBROT_H
