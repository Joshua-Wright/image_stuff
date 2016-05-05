//
// Created by j0sh on 5/5/16.
//

#ifndef IMAGE_STUFF_VORONOI_H
#define IMAGE_STUFF_VORONOI_H

#include "types.h"

namespace image_utils {
    class voronoi_animation {

        struct voronoi_point {
            vect pos;
            vect velocity;
            RGB color;
        };

        std::vector<voronoi_point> points;

        void fill_pixels();

    public:

        matrix<RGB> image_data;

        voronoi_animation(const size_t x, const size_t y, const size_t n_points);

        void advance(const double dt);
    };
}

#endif //IMAGE_STUFF_VORONOI_H
