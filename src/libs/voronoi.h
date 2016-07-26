//
// Created by j0sh on 5/5/16.
//

#ifndef IMAGE_STUFF_VORONOI_H
#define IMAGE_STUFF_VORONOI_H

#include <vector>
#include "types.h"

namespace image_utils {

    enum distance_type {
        EUCLIDEAN,
        MANHATTAN
    };

    class voronoi_animation {

        struct voronoi_point {
            vec pos;
            vec velocity;
            RGB color;
        };

        std::vector<voronoi_point> points;
        const size_t size_x;
        const size_t size_y;

    public:

        voronoi_animation(const size_t x, const size_t y,
                          const size_t n_points);

        image_RGB get_advanced_to_time(const double time) const;

    };


    image_RGB voronoi_filter(const image_RGB &in, const size_t n_points,
                             const distance_type dist = EUCLIDEAN);

    image_RGB fast_voronoi_filter(const image_RGB &in, const size_t n_points);

}

#endif //IMAGE_STUFF_VORONOI_H
