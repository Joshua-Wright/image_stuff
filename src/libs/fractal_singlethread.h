// (c) Copyright 2016 Josh Wright
#ifndef IMAGE_STUFF_FRACTAL_SINGLETHREAD_H
#define IMAGE_STUFF_FRACTAL_SINGLETHREAD_H

#include <map>
#include "types.h"
#include "fractal_common.h"

namespace image_utils {


    class fractal_singlethread {

        vec4 bounds{-2, 2, -2, 2};

        rectangle_stack stack = rectangle_stack(256);

        void process_rectangle(const rectangle &r);

        bool process_line(const line &l);

        double iterate_cell(const complex pos);

    public:
//        mandelbrot_polynomial_t polynomial = func_standard;
        mandelbrot_polynomial_t polynomial;
        matrix<double> iterations;
        size_t max_iterations = 512;
        double pixel_width_x;
        double pixel_width_y;
        bool do_grid = false;
        bool is_julia = false;
        bool smooth = false;
        bool do_sine_transform = true;
        bool subsample = false;
        double mul = 1;
        complex c = complex(0.0, 0);


        fractal_singlethread(const size_t w, const size_t h);

        void set_zoom(vec2 center, double zoom);

        matrix<double> run();

    };
}


#endif //IMAGE_STUFF_FRACTAL_SINGLETHREAD_H
