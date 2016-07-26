// (c) Copyright 2016 Josh Wright
#ifndef IMAGE_STUFF_FRACTAL_SMOOTH_H
#define IMAGE_STUFF_FRACTAL_SMOOTH_H

#include "types.h"

namespace image_utils {

    class fractal_smooth {
        matrix<double> iteration_grid;
        std::array<double, 4> bounds;
        size_t max_iterations = 1024;
        double color_multiplier = 0.15;
        complex c;
        bool is_julia = false;


        double iterate_cell(const complex pos);



    public:

        fractal_smooth(const size_t w, const size_t h, const complex &c = complex(0,0));

        matrix<double> run();

        void set_max_iterations(size_t max_iterations);

        void set_is_julia(bool is_julia);

        void set_bounds(const std::array<double, 4> &bounds);

        void set_color_multiplier(double color_multiplier);

    public:
    };

}

#endif //IMAGE_STUFF_FRACTAL_SMOOTH_H
