// (c) Copyright 2016 Josh Wright
#include <cmath>
#include <iostream>
#include "types.h"
#include "fractal_smooth.h"

namespace image_utils {
    const size_t NOT_DEFINED = (const size_t) -1;

    auto func_standard_plane = [](const complex &z, const complex c) { return pow(z, 2) + c; };

    template<typename T>
    double fractal(complex z, const complex &c, const size_t max_iterations, const T func = func_standard_plane) {
        for (size_t i = 0; i < max_iterations; i++) {
            z = func(z, c);
            if (norm(z) > max_iterations * max_iterations) {
                return i - log2(log2(norm(z))) + 4.0;
            }
        }
        return 0.0;
    }

    double fractal_smooth::iterate_cell(const complex pos) {
        /*TODO: rectangle method for interior*/
        if (is_julia) {
            return fractal(pos, c, max_iterations, func_standard_plane);
        } else {
            return fractal(complex(0, 0), pos, max_iterations, func_standard_plane);
        }
    }


    void fractal_smooth::set_max_iterations(size_t max_iterations) {
        fractal_smooth::max_iterations = max_iterations;
    }

    void fractal_smooth::set_is_julia(bool is_julia) {
        fractal_smooth::is_julia = is_julia;
    }

    fractal_smooth::fractal_smooth(const size_t w, const size_t h, const complex &c) : iteration_grid(w, h, NOT_DEFINED), c(c) {}

    matrix<double> fractal_smooth::run() {
        matrix<double> out(iteration_grid.x(), iteration_grid.y());

#pragma omp parallel for schedule(static) collapse(2)
        for (size_t i = 0; i < out.x(); i++) {
            for (size_t j = 0; j < out.y(); j++) {
                complex pos(
                        (i * 1.0 / out.x()) * (bounds[1] - bounds[0]) + bounds[0],
                        (j * 1.0 / out.y()) * (bounds[3] - bounds[2]) + bounds[2]
                );
                out(i, j) = pow(sin(iterate_cell(pos) * color_multiplier), 2);
            }
        }
        return out;
    }

    void fractal_smooth::set_bounds(const std::array<double, 4> &bounds) {
        this->bounds = bounds;
    }

    void fractal_smooth::set_color_multiplier(double color_multiplier) {
        fractal_smooth::color_multiplier = color_multiplier;
    }
};