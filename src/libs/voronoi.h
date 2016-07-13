//
// Created by j0sh on 5/5/16.
//

#ifndef IMAGE_STUFF_VORONOI_H
#define IMAGE_STUFF_VORONOI_H

#include <vector>
#include <list>
#include "types.h"

namespace image_utils {

    enum distance_type {
        EUCLIDIAN,
        MANHATTAN
    };

    class voronoi_animation {

        struct voronoi_point {
            vect pos;
            vect velocity;
            RGB color;
        };

        std::vector<voronoi_point> points;
        size_t factor;
        matrix<std::list<voronoi_point>> buckets;

        void fill_pixels();

        voronoi_point find_closest(const matrix<std::list<voronoi_point>> &buckets, const size_t factor, const vect_l pt);

    public:

        matrix<RGB> image_data;

        voronoi_animation(const size_t x, const size_t y,
                          const size_t n_points);

        void advance(const double dt);
    };


    image_RGB voronoi_filter(const image_RGB &in, const size_t n_points,
                             const distance_type dist = EUCLIDIAN);

}

#endif //IMAGE_STUFF_VORONOI_H
