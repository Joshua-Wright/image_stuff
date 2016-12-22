// (c) Copyright 2016 Josh Wright
#ifndef IMAGE_STUFF_VORONOI_H
#define IMAGE_STUFF_VORONOI_H

#include <algorithm>
#include <iostream>
#include <vector>
#include "types.h"

namespace image_utils {

    class voronoi {

    public:
        struct cell {
            double dist2;
            size_t point_index;
        };

    private:
        std::vector<vec_ull> points;
        matrix<cell> grid;

        void calculate_distances();

        void flood_out_new_point(const vec_ull &pos, const vec_ull &p);

    public:
        voronoi(const size_t x, const size_t y);

        void calculate(const std::vector<vec_ull> &pts);

        void add_point(const vec_ull &p);

        void into_image(image_RGB &img, const std::vector<RGB> &colors);

        void into_image_averaging(image_RGB &img, const image_RGB &base);

    };

    size_t closest_point(const vec_ull pos, const std::vector<vec_ull> &points);

};

#endif //IMAGE_STUFF_VORONOI_H
