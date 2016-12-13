// (c) Copyright 2016 Josh Wright

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <iomanip>
#include <fractal/fractal_animation_zoom.h>
#include "util/arg_parser.h"

int main(int argc, char const *argv[]) {
    using namespace image_utils;
    help_printer(argc, argv, {
            {"x, y",      "width, height"},
            {"n_frames",  "nubmer of frames to render"},
            {"r, i",      "center to zoom into"},
            {"cr, ci",    "julia initial value"},
            {"mul",       "distance multiplier"},
            {"subsample", "split each pixel 2x2 and average"},
            {"iter",      "number of iteraitons"},
            {"smooth",    "smooth between iterations"},
            {"output",    "output file to write to"},
            {"color",     "colormap to use"},
            {"max_zoom",  ""},
            {"skip",      "skip number of frames at beginning"},
    }, 3, 10);
    arg_parser args(argc, argv);

    auto animation_zoom = make_shared<fractal_animation_zoom>(
            args.read("x", 500),
            args.read("y", 500)
    );
    args.read_into(animation_zoom->max_zoom, "max_zoom", 1e13);
    animation_zoom->center = vec2{
            args.read<double>("r", -0.743643887037151),
            args.read<double>("i", 0.131825904205330),
    };
    animation_zoom->cmap = read_colormap_from_string(args.read<std::string>("color", "hot"));
    animation_zoom->cmap.black_zero = false;

    args.read_bool(animation_zoom->prototype.smooth, "smooth");
    args.read_into(animation_zoom->prototype.max_iterations, "iter", 2048);
    args.read_into(animation_zoom->prototype.mul, "mul", 2);

    /* TODO subsampling render decorator */

    fractal_animator animator(animation_zoom);
    args.read_into(animator.n_frames, "n_frames", 200);
    args.read_into(animator.progress, "skip", 0);

    /*todo create output folder if it doesn't exist*/
    auto output_folder = args.read<std::string>("output_folder", "zoom_frames");
    if (output_folder.back() != '/') {
        output_folder.push_back('/');
    }
    animator.output_folder = output_folder;
    animator.run();

    return 0;
}
