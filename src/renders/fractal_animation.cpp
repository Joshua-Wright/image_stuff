// (c) Copyright 2016 Josh Wright

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <iomanip>
#include <fractal_singlethread.h>
#include "util/arg_parser.h"
#include "colormaps.h"
#include "generators.h"
#include "debug.h"
#include "io.h"
#include "fractal_multithread.h"


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
    config["x"] = "1280";
    config["y"] = "720";
//    config["y"] = "1280";
    config["xa"] = "-2";
    config["xb"] = "2";
    config["ya"] = "-2";
    config["yb"] = "2";
    config["cr"] = "-0.7754659321544456";
    config["ci"] = "0.21015827865930042";
    config["n_frames"] = "200";
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

    // TODO read from arg
    colormap cmap = read_colormap_from_string("rainbow");
    cmap.black_zero = true;

    wave sinewave(wave::SINE);
    fractal_singlethread fractal1(x, y);
    fractal1.max_iterations = iter;
    fractal1.is_julia = false;
//    complex c = complex_circle(center, 0.015, 3.0 * t);
//    fractal1.c = c;
    fractal1.mul = 3;
    fractal1.smooth = true;
    fractal1.do_grid = false;
    fractal1.do_sine_transform = false;

//    fractal1.set_zoom(vec2{1.8, 0.0}, 0.6);
//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "IncompatibleTypes"
//    fractal1.polynomial = func_inv_c;
//#pragma clang diagnostic pop
    fractal1.set_zoom(vec2{0.0, 0.0}, 1.5);
#pragma clang diagnostic push
#pragma ide diagnostic ignored "IncompatibleTypes"
    fractal1.polynomial = func_quadratic_rational;
#pragma clang diagnostic pop


    matrix<double> grid1 = fractal1.run();
    log_transform(grid1);

    size_t progress = 0;
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < n_frames + 1; i++) {

        double t = 1.0 * i / n_frames;

        std::stringstream output;
        output << output_folder << "out_frame_" << std::setfill('0') << std::setw(5) << i << ".png";
        std::string out_filename = output.str();

//        matrix<double> grid2(grid.size(), grid.size());
        matrix<double> grid(0, 0);
        grid = grid1;
//        grid2 += t;
        sine_transform(grid, 1 + 0.3 * sinewave(t), t, true);

        scale_grid(grid);
//        color_write_image(grid, [t, cmap](double x1) { return cmap(x1, t); }, out_filename, false);
        color_write_image(grid, cmap, out_filename, false);

        std::cout << "rendered: \t" << progress << "\t/" << n_frames << std::endl;
        ++progress;
    }

    std::cout << "Done! Render using:" << std::endl;
    std::cout << "ffmpeg -framerate 60 -i "
              << output_folder << "out_frame_%05d.png "
              << output_folder << "output.mp4" << std::endl;


    return 0;
}
