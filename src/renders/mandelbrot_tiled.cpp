// (c) Copyright 2015 Josh Wright
#include "colormaps.h"
#include "fractal/fractal_info.h"
#include "fractal/fractal_singlethread.h"
#include "generators.h"
#include "io.h"
#include "util/arg_parser.h"
#include "util/debug.h"
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

static inline int sgn(int i) {
  if (i > 0) {
    return 1;
  } else if (i < 0) {
    return -1;
  } else {
    return 0;
  }
};

static inline bool render_exists(const std::string &path,
                                 const fractal_info &cfg) {
  json expected = json(cfg);
  json actual;
  std::ifstream f(path + ".json");
  if (f.good()) {
    f >> actual;
    return expected.dump() == actual.dump();
  } else {
    return false;
  }
}

int main(int argc, char const *argv[]) {

  using namespace image_utils;
  help_printer(argc, argv,
               {
                   {"x", "width, height of each tile"},
                   {"r, i", "center of middle tile"},
                   {"zoom", "how far to zoom in. (no zoom -> 1)"},
                   {"cr, ci", "julia initial value"},
                   {"mul", "distance multiplier"},
                   {"subsample", "split each pixel 2x2 and average"},
                   {"iter", "number of iteraitons"},
                   {"smooth", "smooth between iterations"},
                   {"output", "output file to write to"},
                   {"color", "colormap to use"},
                   {"n", "how many tiles to use"},
               },
               3, 10);
  arg_parser args(argc, argv);
  fractal_info cfg = parse_args<fractal_info>(argc, argv);
  // only support square images for now
  // cfg.y = cfg.x;
  std::string base_outfile = args.read<std::string>("output", "output");
  size_t n = args.read<size_t>("n", 2);
  cfg.zoom *= n;
  auto bounds = calc_bounds(cfg.x, cfg.y, vec2{cfg.r, cfg.i}, cfg.zoom);
  double width_at_zoom = (bounds[1] - bounds[0]) / 2;
  double height_at_zoom = (bounds[3] - bounds[2]) / 2;

  int np = n / 2;

#pragma omp parallel for schedule(static) collapse(2)
  for (int i = -np; i <= np; i++) {
    for (int j = -np; j <= np; j++) {
      // TODO do not re-create images that are already made with the same
      // parameters
      if (!(n % 2 == 0 && (i == 0 || j == 0))) {
        std::string outfile = base_outfile + "_" + std::to_string(i) + "_" +
                              std::to_string(j) + ".png";
        fractal_info cfg2 = cfg;
        if (n % 2 == 1) {
          cfg2.r += 2 * width_at_zoom * i;
          cfg2.i += 2 * height_at_zoom * j;
        } else {
          cfg2.r += width_at_zoom * i + sgn(i) * (width_at_zoom) * (abs(i) - 1);
          cfg2.i +=
              height_at_zoom * j + sgn(j) * (height_at_zoom) * (abs(j) - 1);
        }

        if (!render_exists(base_outfile, cfg2)) {

          std::cout << "i=" << i << "\t"
                    << "j=" << j << "\t" << json(cfg2) << std::endl;

          fractal_singlethread fractal;
          fractal.read_config(cfg2);
          fractal.run();

          image_sanity_check(fractal.iterations, true);

          color_write_image(fractal.iterations,
                            read_colormap_from_string(cfg2.color), outfile);
          // write metadata file
          std::ofstream f(outfile + ".json");
          f << json(cfg2) << std::endl;
          f.close();
        }
      }
    }
  }

  std::stringstream script;
  script << "montage ";
  for (int j = np; j >= -np; j--) {
    for (int i = -np; i <= np; i++) {
      if (!(n % 2 == 0 && (i == 0 || j == 0))) {
        script << base_outfile << "_" << i << "_" << j << ".png ";
      }
    }
  }
  // tell montage not to put a border between the things
  script << "-geometry " << cfg.x << "x" << cfg.y << "+0+0 ";
  script << base_outfile << ".png" << std::endl;
  std::cout << script.str();
  std::ofstream("combine_" + base_outfile + ".sh") << script.str();
}
