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
#include "fractal.h"

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
    config["xa"] = "-2";
    config["xb"] = "2";
    config["ya"] = "-2";
    config["yb"] = "2";
    config["cr"] = "1";
    config["ci"] = "1";
    config["iter"] = "100";

    containers::parse_args(config, argc, argv);

    /*TODO: help screen*/

    std::string output(config["output"]);
    const size_t x = std::stoull(config["x"]);
    const size_t y = std::stoull(config["y"]);
    const size_t iter = std::stoull(config["iter"]);

    const std::array<double, 4> bounds = {
            std::stod(config["xa"]),
            std::stod(config["xb"]),
            std::stod(config["ya"]),
            std::stod(config["yb"]),
    };

    complex c(std::stod(config["cr"]), std::stod(config["ci"]));

    matrix<double> grid(0, 0);
    if (config.find("julia") == config.end()) {
        grid = fast_mandelbrot(x, y, iter, bounds);
    } else {
        grid = fast_julia(x, y, iter, bounds, c);
    }

    image_sanity_check(grid, true);
    scale_grid(grid);
//    colormap *cmap = new colormap_threecolor();
    colormap *cmap = new colormap_basic_hot();
    color_write_image(grid, cmap, output);
}
