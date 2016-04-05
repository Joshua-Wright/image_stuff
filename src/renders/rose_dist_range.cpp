// (c) Copyright 2016 Josh Wright
#include <string>
#include <iostream>
#include <map>
#include <iomanip>
#include "generators.h"
#include "debug.h"
#include "colormaps.h"
#include "io.h"

#define DEBUG 0


int main(int argc, char const *argv[]) {
    using namespace image_utils;

#if !DEBUG
    /*TODO: new arg system*/
    if (argc < 4/*TODO: arg count*/) {
        /*0*/ std::cout << argv[0];
        /*1*/ std::cout << " <output folder>";
        /*2*/ std::cout << " <image x>";
        /*3*/ std::cout << " <image y>";
        /*4*/ std::cout << " <n>";
        /*5*/ std::cout << " <d>";
        /*6*/ std::cout << " [wave size]";
        /*7*/ std::cout << " [wave type]";
        /*8*/ std::cout << " [number of frames]";
//        /*9*/ std::cout << " [colormap]"; /*TODO: colormap*/
//        /*10*/ std::cout << " [lookup table size]"; /*TODO with default*/
        std::cout << std::endl;
        /*TODO: these defaults*/
//        std::cout << "wave size:         default 16" << std::endl;
//        std::cout << "lookup table size: 2^x, default 20" << std::endl;
        return 1;
    }
    std::string output_folder(argv[1]);
    /*make sure the path ends in a trailing slash*/
    if (output_folder.back() != '/') {
        output_folder.push_back('/');
    }
    size_t x = std::stoull(argv[2]);
    size_t y = std::stoull(argv[3]);
    int n = std::stoi(argv[4]);
    int d = std::stoi(argv[5]);
    double wave_size = std::stod(argv[6]);
    wave w(argv[7]);
    size_t n_frames = std::stoull(argv[8]);
#else
    size_t x = 500;
    size_t y = 500;
    int n = 3;
    int d = 5;
    wave *w = new wave_fourier_square(3);
    double wave_size = 20;
#endif

    matrix<double> grid_distances(x, y);
    matrix<double> grid_scaled(x, y);

    /*TODO: parameterize wave type*/
    wave offset_wave("noop");
    /*TODO: parameterize colormap*/
    colormap_basic_hot cmap;


    std::cout << "filling lookup table" << std::endl;
    distance_wave *rose_dist1 = new rose_dist(wave("noop"),
                                              std::pow(2, 21),
                                              wave_size, n, d);
    image_fill_2d_wave(grid_distances, rose_dist1);

    for (size_t i = 0; i < n_frames; i++) {
        std::stringstream output;
        output << output_folder
        << "out_frame_"
        << std::setfill('0') << std::setw(5) << i << ".png";
        std::string out_filename = output.str();

        std::cout << "rendering: " << out_filename << std::endl;

        double offset = offset_wave(1.0 * i / n_frames) + 2 * i / n_frames;
        image_fill_apply_wave_to_dist(grid_distances, grid_scaled, w, offset);
        color_write_image(grid_scaled, &cmap, out_filename, false);
    }

    std::cout << "Done! Render using:" << std::endl;
    std::cout
    << "ffmpeg -framerate 60 -i "
    << output_folder
    << "out_frame_%05d.png "
    << output_folder
    << "output.mp4"
    << std::endl;


    return 0;
}
