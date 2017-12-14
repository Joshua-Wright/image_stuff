// (c) Copyright 2015 Josh Wright

#include <boost/multiprecision/float128.hpp>
#include <boost/multiprecision/gmp.hpp>
using namespace boost::multiprecision;
using namespace boost::math;
using namespace boost::math::tools;

#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "colormaps.h"
#include "fractal/fractal_common.h"
#include "fractal/fractal_impl.h"
#include "fractal/fractal_info.h"
#include "generators.h"
#include "io.h"
#include "util/arg_parser.h"
#include "util/debug.h"

static inline int sgn(int i) {
  if (i > 0) {
    return 1;
  } else if (i < 0) {
    return -1;
  } else {
    return 0;
  }
};

std::string to_string(const mpf_float &n) {
  std::stringstream ss;
  ss << n;
  return ss.str();
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
  std::string base_outfile = args.read<std::string>("output", "output");
  size_t n = args.read<size_t>("n", 2);
  mpf_float zoom = numeric_from_string<mpf_float>(cfg.zoom) * n;
  mpf_float cfg_r = numeric_from_string<mpf_float>(cfg.r);
  mpf_float cfg_i = numeric_from_string<mpf_float>(cfg.i);
  auto bounds =
      fractal_impl<mpf_float>::calc_bounds(cfg.x, cfg.y, vect<mpf_float, 2>{cfg_r, cfg_i}, zoom);
  mpf_float width_at_zoom = (bounds[1] - bounds[0]) / 2;
  mpf_float height_at_zoom = (bounds[3] - bounds[2]) / 2;

  int np = n / 2;

#pragma omp parallel for schedule(static) collapse(2)
  for (int i = -np; i <= np; i++) {
    for (int j = -np; j <= np; j++) {
      // for even n, skip rendering the middle cross section
      if (!(n % 2 == 0 && (i == 0 || j == 0))) {
        std::string outfile =
            base_outfile + "_" + std::to_string(i) + "_" + std::to_string(j) + ".png";
        fractal_info cfg2 = cfg;
        cfg2.zoom = to_string(zoom);
        if (n % 2 == 1) {
          cfg2.r = to_string(cfg_r + 2 * width_at_zoom * i);
          cfg2.i = to_string(cfg_i + 2 * height_at_zoom * j);
        } else {
          cfg2.r = to_string(cfg_r + width_at_zoom * i + sgn(i) * (width_at_zoom) * (abs(i) - 1));
          cfg2.i = to_string(cfg_i + height_at_zoom * j + sgn(j) * (height_at_zoom) * (abs(j) - 1));
        }

        // do not re-create images that are already made with the same parameters
        if (!render_exists(base_outfile, cfg2)) {
          std::cout << "i=" << i << "\t"
                    << "j=" << j << "\t" << json(cfg2) << std::endl;

          fractal_ref fractal = get_fractal(cfg2);
          fractal->run_multithread();

          image_sanity_check(fractal->iterations, true);

          color_write_image(fractal->iterations, read_colormap_from_string(cfg2.color), outfile);
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
