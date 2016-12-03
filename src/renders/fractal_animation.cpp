// (c) Copyright 2016 Josh Wright

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <iomanip>
#include "util/arg_parser.h"
#include "colormaps.h"
#include "generators.h"
#include "debug.h"
#include "io.h"
#include "fractal.h"


int main(int argc, char const *argv[]) {
    using namespace image_utils;
    using image_utils::matrix;
    using std::cout;
    using std::endl;
    using std::unordered_map;
    using std::string;

    unordered_map<string, string> config;
    config["folder"] = "fractal_frames";
    config["x"] = "500";
    config["y"] = "500";
    config["xa"] = "-2";
    config["xb"] = "2";
    config["ya"] = "-2";
    config["yb"] = "2";
    config["cr"] = "-0.7269";
    config["ci"] = "0.1889";
    config["n_frames"] = "600";
    config["iter"] = "100";
    containers::parse_args(config, argc, argv);

    /*TODO: help screen*/

    /*todo create output folder if it doesn't exist*/
    std::string output_folder = config["folder"];
    if (output_folder.back() != '/') {
        output_folder.push_back('/');
    }
    const size_t n_frames = std::stoull(config["n_frames"]);
    const size_t x = std::stoull(config["x"]);
    const size_t y = std::stoull(config["y"]);
    const size_t iter = std::stoull(config["iter"]);

    complex center(std::stod(config["cr"]), std::stod(config["ci"]));

    size_t progress = 0;
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < n_frames; i++) {

        std::stringstream output;
        output << output_folder << "out_frame_" << std::setfill('0') << std::setw(5) << i << ".png";
        std::string out_filename = output.str();

        complex c = complex_circle(center, 0.05, 1.0 * i / n_frames);

        fractal fractal1(x, y);
        fractal1.set_max_iterations(iter);
        fractal1.set_is_julia(true);
        fractal1.set_c(c);
        fractal1.set_smooth(true);
        fractal1.set_do_grid(false);
        auto grid = fractal1.run();

        scale_grid(grid);
        // TODO read from arg
        colormap cmap = read_colormap_from_string("hot");
        color_write_image(grid, cmap, out_filename, false);

#pragma omp critical
        {
            std::cout << "rendered: \t" << progress << "\t/" << n_frames << std::endl;
            ++progress;
        }
    }

    std::cout << "Done! Render using:" << std::endl;
    std::cout << "ffmpeg -framerate 60 -i "
              << output_folder << "out_frame_%05d.png "
              << output_folder << "output.mp4" << std::endl;


    return 0;
}
