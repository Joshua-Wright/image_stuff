// (c) Copyright 2016 Josh Wright
#ifndef IMAGE_STUFF_ITERATIVE_FILTER_H
#define IMAGE_STUFF_ITERATIVE_FILTER_H

#include <memory>
#include "voronoi.h"

namespace image_utils {

    class voronoi_state {

        size_t x, y;
        voronoi internal;
        const image_RGB &target;

        std::minstd_rand gen;

    public:
        voronoi_state(const image_RGB &target, const size_t x, const size_t y, const std::vector<vec_ull> &pts);

        double fitness() const;

        voronoi_state child(double normalized_deviation);
    };

    typedef std::shared_ptr<voronoi_state> voronoi_state_ref;

    class iterative_filter {

    };

    /**
     * continuously tries to find the best approximation, and writes each candidate best as it goes
     */
    void brute_force_iterative_filter(const image_RGB &target, const std::string &outf, const size_t n, bool dist_named);


}


#endif //IMAGE_STUFF_ITERATIVE_FILTER_H
