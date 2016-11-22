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
    unordered_map<string, string> config{
            {"output", "output.png"},
            {"x",      "500"},
            {"y",      "500"},
            {"cr",     "-0.7269"},
            {"ci",     "0.1889"},
            {"r",      "0"},
            {"i",      "0"},
            {"zoom",   "1"},
            {"mul",    "1"},
            {"iter",   "100"},
            {"poly",   "std"},
    };

    containers::parse_args(config, argc, argv);

    /*TODO: help screen*/

    const bool smooth = config.find("smooth") != config.end();
    const bool do_grid = config.find("grid") != config.end();
    const bool is_julia = config.find("julia") != config.end();
    const bool subsample = config.find("subsample") != config.end();

    std::string output(config["output"]);
    const size_t x = std::stoull(config["x"]);
    const size_t y = std::stoull(config["y"]);
    const size_t iter = std::stoull(config["iter"]);
    complex c(std::stod(config["cr"]), std::stod(config["ci"]));
    const double zoom = std::stod(config["zoom"]);
    const double mul = std::stod(config["mul"]);
    vec2 center{std::stod(config["r"]), std::stod(config["i"])};

    fractal fractal1(x, y);
    fractal1.set_max_iterations(iter);
    fractal1.set_is_julia(is_julia);
    fractal1.set_c(c);
    fractal1.set_zoom(center, zoom);
    fractal1.set_subsample(subsample);
    fractal1.set_smooth(smooth);
    fractal1.set_do_grid(do_grid);
    fractal1.set_polynomial(config["poly"]);
    fractal1.set_mul(mul);
    auto grid = fractal1.run();

    image_sanity_check(grid, true);
    scale_grid(grid);
//    colormap *cmap = new colormap_threecolor();
    colormap *cmap = new colormap_basic_hot();
//    colormap *cmap = &colormap_3d_cosine::blue_yellow;
//    colormap *cmap = &colormap_simple_gradient::hsv_ish;
//    colormap *cmap = new colormap_grayscale;
//    colormap *cmap = &colormap_gradient::blue_yellow_gradient;
    color_write_image(grid, cmap, output);
//    delete cmap;
}
