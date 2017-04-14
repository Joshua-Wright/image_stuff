// (c) Copyright 2016 Josh Wright
#include "colormaps.h"
#include "util/arg_parser.h"
#include <io.h>
#include <iostream>
#include <string>
#include <vector>


using std::vector;
using std::string;

vector<string> default_colormaps_str{
    "hot",
    "rainbow",
    "gray",
    "threecolor",
    "sine",
    "cubic_blue_yellow",
    "3d-cosine",
};


int main(int argc, char const *argv[]) {
    using namespace image_utils;
    using namespace util;
    arg_parser args(argc, argv);

    auto colorband_width = args.read<size_t>("colorband_width", 1024);
    auto colorband_height = args.read<size_t>("colorband_height", colorband_width / default_colormaps_str.size());


    vector<colormap> colormaps;
    colormaps.reserve(default_colormaps_str.size());
    for (auto &cmap_str : default_colormaps_str) {
        colormaps.push_back(read_colormap_from_string(cmap_str, colorband_width));
        colormaps.back().black_zero = false;
    }

    image_RGB main_image(colorband_width, colorband_height * colormaps.size());
    for (size_t colormap_idx = 0; colormap_idx < colormaps.size(); colormap_idx++) {
        for (size_t column = 0; column < colorband_width; column++) {
            double x = column * 1.0 / colorband_width;
            RGB color = colormaps[colormap_idx](x, 0);
            for (size_t colorband_row = 0; colorband_row < colorband_height; colorband_row++) {
                main_image(column, colormap_idx * colorband_height + colorband_row) = color;
            }
        }
    }

    write_image(main_image, args.read<std::string>("output", "colormap_chart.png"));

    return 0;
}
