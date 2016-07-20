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
    config["cr"] = "-0.7269";
    config["ci"] = "0.1889";
    config["mul"] = "1";
    config["iter"] = "100";

    containers::parse_args(config, argc, argv);

    /*TODO: help screen*/

    std::string output(config["output"]);
    const size_t x = std::stoull(config["x"]);
    const size_t y = std::stoull(config["y"]);
    const size_t iter = std::stoull(config["iter"]);
    const bool do_grid = config.find("grid") != config.end();
    const double color_multiplier = std::stod(config["mul"]);

    const std::array<double, 4> bounds = {
            std::stod(config["xa"]),
            std::stod(config["xb"]),
            std::stod(config["ya"]),
            std::stod(config["yb"]),
    };
    check_bounds(bounds);

    complex c(std::stod(config["cr"]), std::stod(config["ci"]));

    matrix<double> grid(0, 0);
    if (config.find("julia") == config.end()) {
        grid = fast_mandelbrot(x, y, iter, bounds, do_grid);
    } else {
        grid = fast_julia(x, y, iter, bounds, c, do_grid);
    }

    image_sanity_check(grid, true);
    grid *= color_multiplier;
    grid.fmod_in_place(iter);
    scale_grid(grid);
//    scale_iteration_grid_histogram(grid, iter);
//    colormap *cmap = new colormap_threecolor();
    colormap *cmap = new colormap_basic_hot();
//    colormap *cmap = &colormap_gradient::blue_yellow_gradient;
    color_write_image(grid, cmap, output);
}
