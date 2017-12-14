// (c) Copyright 2015 Josh Wright
#include "filters.h"
#include "io.h"
#include "util/arg_parser.h"
#include "util/struct_tuple.h"

using std::string;

struct CFG {
  string in, out;
  string kernel = "unsharp";
};

ADAPT_FIELDS(CFG, in, out, kernel)

int main(int argc, char const **argv) {
  using namespace image_utils;
  using namespace util;
  help_printer(argc, argv,
               {
                   {"in", "input filename"}, {"out", "output filename"},
               });
  CFG cfg = parse_args<CFG>(argc, argv);
  if (cfg.out == "") {
    cfg.out = cfg.in + "blurred.png";
  }

  write_image(
      // image_vec3_to_RGB(gaussian_blur(image_RGB_to_vec3(read_image(cfg.in)))),
      image_vec3_to_RGB(convolve(image_RGB_to_vec3(read_image(cfg.in)),
                                 kernels.at(cfg.kernel))),
      cfg.out);
  return 0;
}
