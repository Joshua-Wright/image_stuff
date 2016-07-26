// (c) Copyright 2016 Josh Wright
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <iomanip>
#include <fractal.h>
#include <util/debug.h>
#include "arg_parser.h"
#include "colormaps.h"
#include "generators.h"
#include "io.h"
#include "fractal_smooth.h"


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

    std::string output(config["output"]);
    const size_t x = std::stoull(config["x"]);
    const size_t y = std::stoull(config["y"]);
    const size_t iter = std::stoull(config["iter"]);
    const bool do_grid = config.find("grid") != config.end();
    const double color_multiplier = std::stod(config["mul"]);
    complex c(std::stod(config["cr"]), std::stod(config["ci"]));
    const std::array<double, 4> bounds = {
            std::stod(config["xa"]),
            std::stod(config["xb"]),
            std::stod(config["ya"]),
            std::stod(config["yb"]),
    };
    check_bounds(bounds);

    fractal_smooth fractal(x, y, c);
    fractal.set_bounds(bounds);
    fractal.set_max_iterations(iter);
    fractal.set_is_julia(config.find("julia") != config.end());

    matrix<double> grid = fractal.run();

    image_sanity_check(grid, true);
    scale_grid(grid);
//    colormap *cmap = new colormap_basic_hot();
//    colormap *cmap = &colormap_gradient::blue_yellow_gradient;
    colormap *cmap = &colormap_3d_cosine::blue_yellow;
    color_write_image(grid, cmap, output);
}
