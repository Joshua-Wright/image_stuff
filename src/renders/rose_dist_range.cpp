// (c) Copyright 2015 Josh Wright
#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include "../libs/generators.h"
#include "../libs/cpp_containers/lib/debug.h"
#include "../libs/colormaps.h"
#include "../libs/io.h"

#define DEBUG 0


int main(int argc, char const *argv[]) {
    using namespace image_utils;

    size_t x = 500;
    size_t y = 500;
    matrix<double> grid_distances(x, y);
    matrix<double> grid_scaled(x, y);

    int n = 3;
    int d = 5;
    wave *w = new wave_fourier_square(3);
//    wave *w = new wave_sawtooth();

//    wave *offset_wave = new wave_sine();
    wave *offset_wave = new wave_fourier_square(3);
    double wave_size = 20;
    colormap *cmap = new colormap_basic_hot();


    std::cout << "filling lookup table" << std::endl;
    rose_dist *rose_dist1 = new rose_dist(new wave_noop(), n, d,
                                          std::pow(2, 21),
                                          wave_size);
    image_fill_2d_wave(grid_distances, rose_dist1);

    size_t max = 100;
    for (size_t i = 0; i < max; i++) {
        std::stringstream output;
        output << "out_frame_";
        output << std::setfill('0') << std::setw(5) << i << ".png";
        std::string out_filename = "frames/";
        out_filename += output.str();
        std::cout << "rendering: " << out_filename << std::endl;

//        double offset = 1.0 * i / max;
        double offset = (*offset_wave)(1.0 * i / max) + 2 * i / max;
        image_fill_apply_range_to_dist(grid_distances, grid_scaled, w, offset);
        color_write_image(grid_scaled, cmap, out_filename);
    }


    return 0;
}
