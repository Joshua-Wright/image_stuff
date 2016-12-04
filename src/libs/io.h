// (c) Copyright 2016 Josh Wright
#pragma once

#include <string>
#include "types.h"
#include "colormaps.h"
#include "generators.h"

namespace image_utils {

    void write_image(const image_RGB &rgb_data, const std::string &out_filename);

    image_RGB read_image(const std::string &filename);

    void image_sanity_check(const matrix<double> &grid, bool print_minmax = false);

    void color_write_image(matrix<double> &grid, const colormap_func &cmap, const std::string &output_filename, bool write_save = true);

    void scale_grid(matrix<double> &grid);

    void scale_iteration_grid_histogram(matrix<double> &grid, const size_t bins);

}
