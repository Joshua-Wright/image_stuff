// (c) Copyright 2016 Josh Wright

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <iomanip>
#include <fractal/fractal_animation_zoom.h>
#include "util/arg_parser.h"
#include "colormaps.h"
#include "generators.h"
#include "debug.h"
#include "io.h"
#include "fractal/fractal_multithread.h"


int main(int argc, char const *argv[]) {
    using namespace image_utils;
    using image_utils::matrix;
    using std::cout;
    using std::endl;
    using std::unordered_map;
    using std::string;

    unordered_map<string, string> config;
    config["folder"] = "zoom_frames";
    config["x"] = "500";
    config["y"] = "500";
    config["xa"] = "-2";
    config["xb"] = "2";
    config["skip"] = "0";
    config["ya"] = "-2";
    config["yb"] = "2";
    config["r"] = "-0.743643887037151";
    config["i"] = " 0.131825904205330";
    config["n_frames"] = "50000";
//    config["n_frames"] = "50000";
    config["iter"] = "256";
    containers::parse_args(config, argc, argv);

    /*TODO: help screen*/

    /*todo create output folder if it doesn't exist*/
    std::string output_folder = config["folder"];
    if (output_folder.back() != '/') {
        output_folder.push_back('/');
    }
    const size_t n_frames = std::stoull(config["n_frames"]);
    const size_t skip = std::stoull(config["skip"]);
    const size_t x = std::stoull(config["x"]);
    const size_t y = std::stoull(config["y"]);
    const size_t iter = std::stoull(config["iter"]);
    vec2 center{std::stod(config["r"]), std::stod(config["i"])};

    // beyond this, doubles aren't good enough
    const double max_zoom = 1e13;


    colormap cmap = read_colormap_from_string("hot");
    cmap.black_zero = true;

    auto animation_zoom = make_shared<fractal_animation_zoom>();
    animation_zoom->x = x;
    animation_zoom->y = y;
    animation_zoom->center = center;
    animation_zoom->cmap = cmap;
    animation_zoom->max_zoom = max_zoom;
    animation_zoom->iter = 2048;

    fractal_animator animator(animation_zoom);
    animator.n_frames = 2;
    animator.output_folder = output_folder;
    animator.run();

    return 0;
}
