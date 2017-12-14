// (c) Copyright 2016 Josh Wright
#pragma once

#include "voronoi/voronoi.h"
#include "types.h"

namespace image_utils {
    double avg_sq_dist_same_size(const image_RGB &image1, const image_RGB &image2);

    double avg_sq_dist_same_size(const voronoi &voronoi1, const image_RGB &base);

    // TODO non-same-size using histograms
}