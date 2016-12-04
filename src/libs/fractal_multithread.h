//
// Created by j0sh on 5/27/16.
//

#ifndef IMAGE_STUFF_MANDELBROT_H
#define IMAGE_STUFF_MANDELBROT_H

#include <map>
#include "types.h"
#include "fractal_common.h"

namespace image_utils {

    class fractal_multithread {
    public:
    private:

        struct split_rectangle {
            // basically an option type holding either 4 rectangles or nothing
            bool did_split;
            rectangle rectangles[4];
        };

        matrix<double> iterations;
        matrix<bool> grid_mask;
        size_t max_iterations = 512;
        vec4 bounds{-2, 2, -2, 2};
        double pixel_width_x;
        double pixel_width_y;
        bool do_grid = false;
        bool is_julia = false;
        bool smooth = false;
        bool do_sine_transform = true;
        bool subsample = false;
        double mul = 1;
        polynomial_t polynomial = STANDARD;
        complex c = complex(0.0, 0);

        double iterate_cell(const complex pos);

        bool process_line(const line &l);

        split_rectangle process_rectangle(rectangle r);


    public:

        fractal_multithread(const size_t w, const size_t h);

        void set_max_iterations(size_t max_iterations);

        void set_do_grid(bool do_grid);

        void set_is_julia(bool is_julia);

        void set_smooth(bool smooth);

        void set_do_sine_transform(bool do_sine_transform);

        void set_subsample(bool subsample);

        void set_c(const complex &c);

        void set_zoom(vec2 center, double zoom);

        void set_polynomial(polynomial_t polynomial);

        void set_polynomial(const std::string &name);

        void set_mul(double mul);

        matrix<double> run();
    };

    /** t on range [0,1]*/
    complex complex_circle(const complex center, const double r, const double t);
}
#endif //IMAGE_STUFF_MANDELBROT_H
