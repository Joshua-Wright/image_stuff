// (c) Copyright 2016 Josh Wright
#include <iostream>
#include <string>
#include <cstring>
#include "io.h"
#include "lodepng.h"

namespace image_utils {

    void write_image(image_RGB &rgb_data, const std::string &out_filename) {
        std::vector<unsigned char> rgb_image_data(3 * rgb_data.size());
#pragma omp parallel for schedule(static) collapse(2)
        for (size_t i = 0; i < rgb_data.x(); i++) {
            for (size_t j = 0; j < rgb_data.y(); j++) {
                rgb_image_data[3 * (j * rgb_data.x() + i) + 0] = rgb_data(i,
                                                                          j).r;
                rgb_image_data[3 * (j * rgb_data.x() + i) + 1] = rgb_data(i,
                                                                          j).g;
                rgb_image_data[3 * (j * rgb_data.x() + i) + 2] = rgb_data(i,
                                                                          j).b;
            }
        }
        lodepng::encode(out_filename, rgb_image_data, rgb_data.x(),
                        rgb_data.y(), LCT_RGB);
    };

    matrix<RGB> read_image(const std::string &filename) {
        std::vector<unsigned char> data;
        unsigned w, h;
        lodepng::decode(data, w, h, filename, LCT_RGB);
        matrix<RGB> output(w, h);
#pragma omp parallel for schedule(static) collapse(2)
        for (size_t i = 0; i < w; i++) {
            for (size_t j = 0; j < h; j++) {
                output(i, j).r = data[3 * (j + i * h) + 0];
                output(i, j).g = data[3 * (j + i * h) + 1];
                output(i, j).b = data[3 * (j + i * h) + 2];
            }
        }
        return output;
    }

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
                           const std::string &output_filename,
                           bool write_save) {
        /* modifies argument! */
        scale_grid(grid);
        image_RGB color_image(grid.x(), grid.y());
        grayscale_to_rgb(grid, color_image, cmap);
        if (write_save) {
            std::cout << "saving image" << std::endl;
        }
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