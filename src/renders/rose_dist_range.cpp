// (c) Copyright 2016 Josh Wright
#include <string>
#include <iostream>
#include <map>
#include <iomanip>
#include "generators.h"
#include "util/debug.h"
#include "colormaps.h"
#include "util/arg_parser.h"
#include "io.h"


int main(int argc, char const *argv[]) {
    using namespace image_utils;
    using std::cout;
    using std::endl;
    using std::unordered_map;
    using std::string;
    unordered_map<string, string> config;

    /*default values*/
    config["folder"] = "frames";
    config["x"] = "500";
    config["y"] = "500";
    config["n"] = "3";
    config["d"] = "7";
    config["n_frames"] = "30";
    config["wave_size"] = "16";
    config["wave_type"] = "fourier_square:3";
    config["off_wave"] = "noop";
    config["base_offset"] = "0";
    config["lookup_table_size"] = "20";
    containers::parse_args(config, argc, argv);

    if (argc == 1 ||
        config.find("--help") != config.end() ||
        config.find("-h") != config.end()) {
        /*help text*/
        std::cout << "Usage: " << argv[0] << " [parameter_name=definition ...]"
        << std::endl;
        std::cout << std::endl;
        int pw = 20; /*parameter width*/
        int dw = 80 - pw - 10; /*description width*/
        // @formatter:off
        std::cout << std::setw(pw) <<         "parameter:" << std::setw(dw) <<               "description:" << std::endl;
        std::cout << std::setw(pw) <<             "folder" << std::setw(dw) <<              "output folder" << std::endl;
        std::cout << std::setw(pw) <<           "n_frames" << std::setw(dw) <<           "number of frames" << std::endl;
        std::cout << std::setw(pw) <<                  "x" << std::setw(dw) <<                "image width" << std::endl;
        std::cout << std::setw(pw) <<                  "y" << std::setw(dw) <<               "image height" << std::endl;
        std::cout << std::setw(pw) <<                  "n" << std::setw(dw) <<             "rose parameter" << std::endl;
        std::cout << std::setw(pw) <<                  "d" << std::setw(dw) <<             "rose parameter" << std::endl;
        std::cout << std::setw(pw) <<          "wave_size" << std::setw(dw) <<     "relative size of waves" << std::endl;
        std::cout << std::setw(pw) <<          "wave_type" << std::setw(dw) <<              "type of waves" << std::endl;
        std::cout << std::setw(pw) <<           "off_wave" << std::setw(dw) <<        "type of offset wave" << std::endl;
        std::cout << std::setw(pw) <<        "base_offset" << std::setw(dw) <<       "added to offset wave" << std::endl;
        std::cout << std::setw(pw) <<  "lookup_table_size" << std::setw(dw) <<  "size of lookup table size" << std::endl;
        std::cout << std::setw(pw + dw) << "(given as 2^x)" << std::endl;
        // @formatter:on
        return 0;
    }


    const size_t x = std::stoull(config["x"]);
    const size_t y = std::stoull(config["y"]);
    const int n = std::stoi(config["n"]);
    const int d = std::stoi(config["d"]);
    const double wave_size = std::stod(config["wave_size"]);
    const double base_offset = std::stod(config["base_offset"]);
    const wave w(config["wave_type"]);
    const wave offset_wave(config["off_wave"]);
    const size_t n_frames = std::stoull(config["n_frames"]);
    size_t table_size2 = std::stoull(config["lookup_table_size"]);

    std::string output_folder = config["folder"];
    /*make sure the path ends in a trailing slash*/
    if (output_folder.back() != '/') {
        output_folder.push_back('/');
    }

    matrix<double> grid_distances(x, y);


    /*TODO: parameterize colormap*/
    colormap_basic_hot cmap;


    std::cout << "filling lookup table" << std::endl;
    rose_dist rose_dist1(wave("noop"), std::pow(2, table_size2),
                         wave_size, n, d);
    image_fill_2d_wave(grid_distances, &rose_dist1);

#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < n_frames; i++) {

        std::stringstream output;

        output << output_folder <<
        "out_frame_" << std::setfill('0') <<
        std::setw(5) << i << ".png";

        std::string out_filename = output.str();
        double offset = offset_wave(1.0 * i / n_frames) + 2 * i / n_frames;

        matrix<double> grid_scaled(x, y);
        image_fill_apply_wave_to_dist(grid_distances, grid_scaled, w,
                                      offset + base_offset);

        color_write_image(grid_scaled, &cmap, out_filename, false);
        std::cout << "rendered: " << out_filename << std::endl;

    }

    std::cout << "Done! Render using:" << std::endl;
    std::cout << "ffmpeg -framerate 60 -i "
    << output_folder << "out_frame_%05d.png "
    << output_folder << "output.mp4" << std::endl;

    return 0;
}
