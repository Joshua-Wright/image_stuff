// (c) Copyright 2015 Josh Wright

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <iomanip>
#include "arg_parser.h"
#include "colormaps.h"
#include "generators.h"
#include "io.h"
#include "mandelbrot.h"

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
    config["iter"] = "100";
    config["wtheta"] = "fourier_square:3";

    containers::parse_args(config, argc, argv);

    /*TODO: help screen*/

    std::string output(config["output"]);
    const size_t x = std::stoull(config["x"]);
    const size_t y = std::stoull(config["y"]);
    const size_t iter = std::stoull(config["iter"]);

    matrix<double> grid(x, y);

//    mandelbrot(iter, grid);
    mandelbrot(iter, grid, {-2, 0, -1, 1});
//    mandelbrot(iter, grid, {-2, 2, -2, 2});

    image_sanity_check(grid, true);
    scale_grid(grid);
    image_sanity_check(grid, true);
//    colormap *cmap = new colormap_threecolor();
    colormap *cmap = new colormap_basic_hot();
    color_write_image(grid, cmap, output);
}
