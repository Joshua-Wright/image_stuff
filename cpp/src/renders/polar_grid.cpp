// (c) Copyright 2016 Josh Wright
#include "colormaps.h"
#include "generators.h"
#include "io.h"
#include "util/arg_parser.h"
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    using image_utils::matrix;
    using std::cout;
    using std::endl;
    using std::unordered_map;
    using std::string;


    /*output*/
    unordered_map<string, string> config;
    config["output"] = "output.png";
    config["x"] = "500";
    config["y"] = "500";
    config["xdist"] = "3";
    config["xtheta"] = "10";
    config["wdist"] = "sine";
    config["wtheta"] = "fourier_square:3";

    parse_args(config, argc, argv);


    if (argc == 1 ||
        config.find("--help") != config.end() ||
        config.find("-h") != config.end()) {
        /*help text*/
        std::cout << "Usage: " << argv[0] << " [parameter_name=definition ...]"
                  << std::endl;
        std::cout << std::endl;
        int pw = 20;           /*parameter width*/
        int dw = 80 - pw - 10; /*description width*/
        // @formatter:off
        std::cout << std::setw(pw) << "parameter:" << std::setw(dw) << "description:" << std::endl;
        std::cout << std::setw(pw) << "output" << std::setw(dw) << "output filename" << std::endl;
        std::cout << std::setw(pw) << "x" << std::setw(dw) << "image width" << std::endl;
        std::cout << std::setw(pw) << "y" << std::setw(dw) << "image height" << std::endl;
        std::cout << std::setw(pw) << "xdist" << std::setw(dw) << "distance multiplier" << std::endl;
        std::cout << std::setw(pw) << "xtheta" << std::setw(dw) << "theta multiplier" << std::endl;
        std::cout << std::setw(pw) << "wdist" << std::setw(dw) << "distance wave" << std::endl;
        std::cout << std::setw(pw) << "wtheta" << std::setw(dw) << "theta wave" << std::endl;
        // @formatter:on
        return 0;
    }

    /*TODO: print help text if (argc == 0) or help parameter*/

    std::string output(config["output"]);
    const size_t x = std::stoull(config["x"]);
    const size_t y = std::stoull(config["y"]);
    const double mul_dist = std::stod(config["xdist"]);
    const double mul_theta = std::stod(config["xtheta"]);
    wave wave_dist(config["wdist"]);
    wave wave_theta(config["wtheta"]);


    /*allocate a grid for the math*/
    matrix<double> grid(y, x);

    /*fill the grid*/
    image_fill_circle_grid(grid, mul_theta, mul_dist, wave_dist, wave_theta);

    /*write the image*/
    //    colormap *cmap = new colormap_basic_hot();
    colormap cmap = read_colormap_from_string("threecolor");
    color_write_image(grid, cmap, output);
}
