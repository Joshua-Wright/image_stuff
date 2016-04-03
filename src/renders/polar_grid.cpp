// (c) Copyright 2016 Josh Wright
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include "arg_parser.h"
#include "colormaps.h"
#include "generators.h"
#include "io.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    using image_utils::matrix;
    using std::cout;
    using std::endl;
    using std::unordered_map;
    using std::string;
    unordered_map<string,string> config;

    /*output*/
    config["output"] = "output.png";
    config["x"] = "500";
    config["y"] = "500";
    config["xdist"] = "3";
    config["xtheta"] = "10";
    config["wdist"] = "sine";
    config["wtheta"] = "fourier_square:3";
    containers::parse_args(config, argc, argv);

    /*TODO: print help text if (argc == 0) or help parameter*/

    std::string output(config["output"]);
    const size_t x = std::stoull(config["x"]);
    const size_t y = std::stoull(config["y"]);
    const double mul_dist = std::stod(config["xdist"]);
    const double mul_theta = std::stod(config["xtheta"]);
    wave *wave_dist = parse_wave_spec(config["wdist"]);
    wave *wave_theta = parse_wave_spec(config["wtheta"]);


    /*allocate a grid for the math*/
    matrix<double> grid(y, x);

    /*fill the grid*/
    image_fill_circle_grid(grid, mul_theta, mul_dist, wave_dist, wave_theta);

    delete wave_dist;
    delete wave_theta;

    /*write the image*/
//    colormap *cmap = new colormap_basic_hot();
    colormap *cmap = new colormap_threecolor();
    color_write_image(grid, cmap, output);
}
