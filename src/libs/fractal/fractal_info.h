#pragma once
#include "util/struct_tuple.h"

struct fractal_info {
  size_t x = 500,
         y = 500;
  size_t iter = 256;
  double r = 0,
         i = 0;
  double cr = 0,
         ci = 0;
  double zoom = 1;
  double mul = 1;
  bool subsample = false;
  bool smooth = false;
  bool do_grid = false;
  bool is_julia = false;
  std::string color = "hot";
  std::string poly;
};

ADAPT_FIELDS(fractal_info,
             x, y,
             iter,
             r, i,
             cr, ci,
             zoom,
             mul,
             subsample,
             smooth,
             do_grid,
             is_julia,
             color,
             poly)
