// (c) Copyright 2016 Josh Wright

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <iomanip>
#include <fractal/fractal_singlethread.h>
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
    config["folder"] = "fractal_frames";
    config["x"] = "500";
    config["y"] = "500";
//    config["x"] = "1280";
//    config["y"] = "720";
//    config["x"] = "1920";
//    config["y"] = "1080";
    config["xa"] = "-2";
    config["xb"] = "2";
    config["ya"] = "-2";
    config["yb"] = "2";
    config["cr"] = "-0.7754659321544456";
    config["ci"] = "0.21015827865930042";
//    config["n_frames"] = "400";
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

    colormap cmap = read_colormap_from_string("rainbow");
    cmap.black_zero = true;

    wave sinewave(wave::SINE);
    fractal_singlethread fractal1(x, y);
    fractal1.max_iterations = iter;
    fractal1.is_julia = false;
    fractal1.mul = 3;
    fractal1.smooth = true;
    fractal1.do_grid = false;
    fractal1.do_sine_transform = false;

//    fractal1.set_zoom(vec2{2.2, 0.0}, 1.2);
//    fractal1.polynomial = INV_C;

    fractal1.set_zoom(vec2{0.0, 0.0}, 1);
    fractal1.polynomial = CUSTOM;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "IncompatibleTypes"
    fractal1.custom_polynomial = [](complex z, complex c) { return std::pow(z, 3) + c; };
#pragma clang diagnostic pop

    fractal1.run();
    matrix<double> &grid1 = fractal1.iterations;
    log_transform(grid1);

    matrix<double> grid(grid1.x(), grid1.y());

    size_t progress = 0;
#pragma omp parallel for schedule(static) private(grid)
    for (size_t i = 0; i < n_frames + 1; i++) {

        double t = 1.0 * i / n_frames;

        std::stringstream output;
        output << output_folder << "out_frame_" << std::setfill('0') << std::setw(5) << i << ".png";
        std::string out_filename = output.str();

//        matrix<double> grid(0, 0);
        if (grid.x() != grid1.x()) {
            grid = grid1;
        }
        std::copy(grid1.begin(), grid1.end(), grid.begin());
        grid = grid1;
//        grid2 += t;
//        sine_transform(grid, 1 + 0.3 * sinewave(t), t, true);
        sine_transform(grid, 2, t, true);

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
