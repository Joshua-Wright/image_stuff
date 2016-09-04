//
// Created by j0sh on 5/27/16.
//

#ifndef IMAGE_STUFF_MANDELBROT_H
#define IMAGE_STUFF_MANDELBROT_H

#include "types.h"

namespace image_utils {

    class fractal {

        struct line {
            vec_ull start_point;
            vec_ull end_point;
        };

        struct rectangle {
            // bounds are inclusive
            // uint16_t so that the whole struct fits in a single word
            uint16_t xmin, xmax, ymin, ymax;
            rectangle();

            rectangle(const uint16_t x_min, const uint16_t x_max,
                      const uint16_t y_min, const uint16_t y_max);

            std::array<fractal::line, 4> get_sides();
        };

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
        complex c = complex(0.0, 0);

        double iterate_cell(const complex pos);

        bool process_line(const line &l);

        split_rectangle process_rectangle(rectangle r);


    public:

        fractal(const size_t w, const size_t h);

        void set_max_iterations(size_t max_iterations);

        void set_do_grid(bool do_grid);

        void set_is_julia(bool is_julia);

        void set_smooth(bool smooth);

        void set_do_sine_transform(bool do_sine_transform);

        void set_subsample(bool subsample);

        void set_c(const complex &c);

        void set_zoom(vec2 center, double zoom);

        matrix<double> run();
    };

    /** bounds is in the form {xmin, xmax, ymin, ymax} */
    void check_bounds(const std::array<double, 4> &bounds);

    /** t on range [0,1]*/
    complex complex_circle(const complex center, const double r, const double t);
}
#endif //IMAGE_STUFF_MANDELBROT_H
