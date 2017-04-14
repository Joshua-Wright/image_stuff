// (c) Copyright 2015 Josh Wright
#include "colormaps.h"
#include "fractal/fractal_multithread.h"
#include "generators.h"
#include "io.h"
#include "util/arg_parser.h"
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main(int argc, char const *argv[]) {
                using namespace image_utils;
                help_printer(argc, argv, {
                                             {"x, y", "width, height"},
                                             {"r, i", "center"},
                                             {"zoom", "how far to zoom in. (no zoom -> 1)"},
                                             {"cr, ci", "julia initial value"},
                                             {"mul", "distance multiplier"},
                                             {"subsample", "split each pixel 2x2 and average"},
                                             {"iter", "number of iteraitons"},
                                             {"smooth", "smooth between iterations"},
                                             {"output", "output file to write to"},
                                             {"color", "colormap to use"},
                                         },
                             3, 10);
                arg_parser args(argc, argv);

                fractal_multithread fractal(
                    args.read<size_t>("x", 800),
                    args.read<size_t>("y", 500));

                fractal.set_zoom(
                    vec2{
                        args.read<double>("r", 0),
                        args.read<double>("i", 0),
                    },
                    args.read<double>("zoom", 1));

                fractal.set_polynomial(args.read<std::string>("poly", "standard"));

                fractal.c = complex(
                    args.read<double>("cr", -0.7269),
                    args.read<double>("ci", 0.1889));

                args.read_bool(fractal.subsample, "subsample");
                args.read_bool(fractal.smooth, "smooth");
                args.read_bool(fractal.do_grid, "do_grid");
                args.read_bool(fractal.is_julia, "julia");

                args.read_into(fractal.max_iterations, "iter", 256);
                args.read_into(fractal.mul, "mul", 1.0);

                fractal.run();

                image_sanity_check(fractal.iterations, true);
                scale_grid(fractal.iterations);

                color_write_image(
                    fractal.iterations,
                    read_colormap_from_string(args.read<std::string>("color", "hot")),
                    args.read<std::string>("output", "output.png"));
}
