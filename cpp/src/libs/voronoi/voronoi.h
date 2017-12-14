// (c) Copyright 2016 Josh Wright
#ifndef IMAGE_STUFF_VORONOI_H
#define IMAGE_STUFF_VORONOI_H

#include <algorithm>
#include <iostream>
#include <vector>
#include "types.h"

namespace image_utils {

    class voronoi {

        friend double avg_sq_dist_same_size(const voronoi &voronoi1, const image_RGB &base);

    public:
        struct cell {
            double dist2;
            size_t point_index;
        };

    private:
        std::vector<vec_ull> _points;
        matrix<cell> grid;

        void calculate_distances();

        void flood_out_new_point(const vec_ull &pos, const vec_ull &p);

    public:
        voronoi(const size_t x, const size_t y);
        voronoi();
        voronoi(const voronoi &rhs);

        void calculate(const std::vector<vec_ull> &pts);

        void add_point(const vec_ull &p);

        void into_image(image_RGB &img, const std::vector<RGB> &colors);

        void into_image_averaging(image_RGB &img, const image_RGB &base);

        std::vector<RGB> cell_average_colors(const image_RGB &base) const;

        const std::vector<vec_ull> &points() const;

    };

    size_t closest_point(const vec_ull pos, const std::vector<vec_ull> &points);

};

#endif //IMAGE_STUFF_VORONOI_H
