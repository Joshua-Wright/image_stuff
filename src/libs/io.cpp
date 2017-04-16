// (c) Copyright 2016 Josh Wright
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <set>
#include <unordered_map>
#include "io.h"
#include "lodepng.h"

namespace image_utils {

    void write_image(const image_RGB &rgb_data, const std::string &out_filename) {
        lodepng::encode(out_filename, (const unsigned char *) rgb_data.data(),
                        rgb_data.x(), rgb_data.y(), LCT_RGB);
    };

    image_RGB read_image(const std::string &filename) {
        std::vector<unsigned char> data;
        unsigned w, h;
        lodepng::decode(data, w, h, filename, LCT_RGB);
        matrix<RGB> output(w, h);
        memcpy(output.data(), data.data(), sizeof(RGB) * w * h);
        return output;
    }

    void image_sanity_check(const matrix<double> &grid, bool print_minmax) {
        /*checks the output to make sure it looks valid*/
        auto min_max_tuple = std::minmax_element(grid.begin(),
                                                 grid.end());
        double min = *min_max_tuple.first;
        double max = *min_max_tuple.second;
        if (min == INF || min == -INF ||
            max == INF || max == -INF ||
            std::isnan(min) || std::isnan(max)) {
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

    void color_write_image(const matrix<double> &grid, const colormap_func &cmap, const std::string &output_filename, bool write_save) {
        image_RGB color_image(grid.x(), grid.y());
        grayscale_to_rgb(grid, color_image, cmap);
        if (write_save) {
            std::cout << "saving image" << std::endl;
        }
        write_image(color_image, output_filename);

    }

    void scale_grid(matrix<double> &grid) {
        auto g = std::minmax_element(grid.begin(), grid.end());
        double min = *g.first;
        double max = *g.second;
        grid -= min;
        grid /= (max - min);
    }

}
