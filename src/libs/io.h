// (c) Copyright 2015 Josh Wright
#pragma once

#include <string>
#include "types.h"
#include "colormaps.h"

namespace image_utils {

    void write_image(image_RGB &rgb_data, const std::string &out_filename);

    void image_sanity_check(const matrix<long double> &doubles,
                            bool print_minmax = false);

    void color_write_image(matrix<long double> &grid, colormap_f colormap,
                           const std::string &output_filename);

    void scale_grid(matrix<long double> &doubles);

}