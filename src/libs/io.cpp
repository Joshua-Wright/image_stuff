// (c) Copyright 2015 Josh Wright
#include <iostream>
#include <string>
#include <cstring>
#include "io.h"
#include "../external_libs/lodepng.h"

namespace image_utils {

    void write_image(image_RGB &rgb_data, const std::string &out_filename) {
        /*reserve 4 bytes per pixel, RGB and transparency*/
        std::vector<unsigned char> rgba_image_data(4 * rgb_data.size());
        for (size_t i = 0; i < rgb_data.size(); ++i) {
            rgba_image_data[4 * i + 0] = rgb_data(i).r;
            rgba_image_data[4 * i + 1] = rgb_data(i).g;
            rgba_image_data[4 * i + 2] = rgb_data(i).b;
            rgba_image_data[4 * i + 3] = 0xff; /*fully opaque*/
        }
        lodepng::encode(out_filename, rgba_image_data, rgb_data.y(),
                        rgb_data.x());
    };

    void image_sanity_check(const matrix<double> &doubles,
                            bool print_minmax) {
        /*checks the output to make sure it looks valid*/
        auto min_max_tuple = std::minmax_element(doubles.begin(),
                                                 doubles.end());
        double min = *min_max_tuple.first;
        double max = *min_max_tuple.second;
        if (min == INFINITY || min == -INFINITY || max == INFINITY ||
            max == -INFINITY || std::isnan(min) ||
            std::isnan(max)) {
            std::cout << "infinity detected" << std::endl;
        }
        if (print_minmax) {
            std::cout << min << " " << max << std::endl;
        }
        if (min == 0 && max == 0) {
            std::cout << "everything is 0" << std::endl;
        } else if (min == max) {
            std::cout << "everything is equal" << std::endl;
        }
    }

    void color_write_image(matrix<double> &grid, colormap *cmap,
                           const std::string &output_filename) {
        /* modifies argument! */
        scale_grid(grid);
        image_RGB color_image(grid.x(), grid.y());
        grayscale_to_rgb(grid, color_image, cmap);
        std::cout << "saving image" << std::endl;
        write_image(color_image, output_filename);

    }

    void scale_grid(matrix<double> &doubles) {
        auto g = std::minmax_element(doubles.begin(), doubles.end());
        double min = *g.first;
        double max = *g.second;
        doubles -= min;
        doubles /= (max - min);
    }

}