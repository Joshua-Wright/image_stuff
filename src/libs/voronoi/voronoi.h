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
        struct point {
            vec_ull position;
            RGB color;
        };
        struct cell {
            double dist2;
            size_t point_index;
        };

    private:
        std::vector<point> points;
        matrix<cell> grid;

        void calculate_distances();

        void flood_out_new_point(const vec_ull &pos, const point &p);

    public:
        voronoi(const size_t x, const size_t y);

        void calculate(const std::vector<point> &pts);

        void add_point(const point &p);

        void into_image(image_RGB &img);

    };

};

#endif //IMAGE_STUFF_VORONOI_H
