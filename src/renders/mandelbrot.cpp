// (c) Copyright 2015 Josh Wright
#include "colormaps.h"
#include "fractal/fractal_info.h"
#include "fractal/fractal_multithread.h"
#include "generators.h"
#include "io.h"
#include "util/arg_parser.h"
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main(int argc, char const *argv[]) {

  using namespace image_utils;
  help_printer(argc, argv,
               {
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
  fractal_info cfg = parse_args<fractal_info>(argc, argv);
  fractal_multithread fractal;
  fractal.read_config(cfg);

  std::cout << json(cfg) << std::endl;

  fractal.run();

  image_sanity_check(fractal.iterations, true);
  scale_grid(fractal.iterations);

  std::string outfile = args.read<std::string>("output", "output.png");
  color_write_image(fractal.iterations, read_colormap_from_string(cfg.color),
                    outfile);
  // write metadata file
  std::ofstream f(outfile + ".json");
  f << json(cfg) << std::endl;
  f.close();
}
