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
#include "fractal_multithread.h"


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
    config["iter"] = "1024";
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
    /* todo: add support for long double and __float128 */

    size_t progress = skip;
    // don't need OpenMP here because the frames themselves are rendered in parallelg
//#pragma omp parallel for schedule(dynamic)
    for (size_t i = skip; i < n_frames; i++) {

        std::stringstream output;
        output << output_folder << "out_frame_" << std::setfill('0') << std::setw(5) << i << ".png";
        std::string out_filename = output.str();

        double zoom = std::exp((1.0 * i / n_frames) * std::log(max_zoom));

        fractal_multithread fractal1(x, y);
        fractal1.set_max_iterations(iter);
        fractal1.set_is_julia(false);
        fractal1.set_zoom(center, zoom);
        fractal1.set_smooth(true);
        // videos look just horrible without subsampling
        fractal1.set_subsample(true);
        fractal1.set_subsample(true);
        fractal1.set_do_grid(false);
        auto grid = fractal1.run();

        scale_grid(grid);
        colormap cmap = read_colormap_from_string("threecolor");
        color_write_image(grid, cmap, out_filename, false);

#pragma omp critical
        {
            std::cout << "rendered: \t" << progress << "\t/" << n_frames
                      << "\t file=" << out_filename << " zoom=" << zoom << std::endl;
            ++progress;
        }
    }

    std::cout << "Done! Render using:" << std::endl;
    std::cout << "ffmpeg -framerate 60 -i "
              << output_folder << "out_frame_%05d.png "
              << output_folder << "output.mp4" << std::endl;


    return 0;
}
