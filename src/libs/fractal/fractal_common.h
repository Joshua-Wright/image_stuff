// (c) Copyright 2016 Josh Wright
#pragma once

#include <util/debug.h>
#include <functional>
#include <map>
#include "fractal_info.h"
#include "types.h"

namespace image_utils {
const double NOT_DEFINED = -1.0;

#define FRACTAL_POLYNOMIAL(name, expr)                                  \
  template <typename numeric>                                           \
  struct name {                                                         \
    typedef std::complex<numeric> complex;                              \
    static std::complex<numeric> func(const std::complex<numeric> &z,   \
                                      const std::complex<numeric> &c) { \
      return expr;                                                      \
    }                                                                   \
  };

FRACTAL_POLYNOMIAL(func_standard, pow(z, 2) + c)
FRACTAL_POLYNOMIAL(func_cubic, pow(z, 3) + c)
FRACTAL_POLYNOMIAL(func_quadratic_rational, pow(z, 2) + pow(c, 2) / (pow(c, 4) - 0.25))
FRACTAL_POLYNOMIAL(func_inv_c, pow(z, 2) + 1.0 / (c - 1.0))
FRACTAL_POLYNOMIAL(func_inv_c_parabola, pow(z, 2) + 1.0 / c + 0.25)

enum polynomial_t {
  STANDARD,
  CUBIC,
  QUADRATIC_RATIONAL,
  INV_C,
  INV_C_PARABOLA,
};
const std::map<std::string, polynomial_t> names{
    {"standard", STANDARD},
    {"cubic", CUBIC},
    {"inv-c", INV_C},
    {"quadratic-rational", QUADRATIC_RATIONAL},
    {"inv-c-parabola", INV_C_PARABOLA},
};

void sine_transform(matrix<double> &in, const double multiplier = 1, const double rel_phase = 0,
                    bool preserve_zero = true);
void log_transform(matrix<double> &in, const double multiplier = 1);

/** t on range [0,1]*/
complex complex_circle(const complex center, const double r, const double t);

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

struct line;

struct rectangle;

struct split_rectangle;

struct rectangle_stack;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

struct line {
  vec_ull start_point;
  vec_ull end_point;
};

struct rectangle {
  size_t xmin, xmax, ymin, ymax;

  rectangle(){};

  rectangle(const size_t x_min, const size_t x_max, const size_t y_min, const size_t y_max)
      : xmin(x_min), xmax(x_max), ymin(y_min), ymax(y_max) {}

  std::array<line, 4> get_sides() const {
    return std::array<line, 4>{line{{xmin, ymin}, {xmax, ymin}}, line{{xmin, ymin}, {xmin, ymax}},
                               line{{xmax, ymin}, {xmax, ymax}}, line{{xmin, ymax}, {xmax, ymax}}};
  };
};

struct split_rectangle {
  // basically an option type holding either 4 rectangles or nothing
  bool did_split;
  rectangle rectangles[4];
};

struct rectangle_stack : public std::vector<rectangle> {
  rectangle_stack(size_t sz) : std::vector<rectangle>() { reserve(sz); };

  rectangle pop() {
    rectangle ret = back();
    pop_back();
    return ret;
  }

  void push(const rectangle &rect) { push_back(rect); }
};

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

template <typename numeric = double>
class fractal_base {
 public:
  typedef vect<numeric, 2> vec2;
  typedef vect<numeric, 3> vec3;
  typedef vect<numeric, 4> vec4;
  typedef std::complex<numeric> complex;

  static vec2 calc_pixel_widths(size_t x, size_t y, const numeric &zoom) {
    numeric dx = 2 / zoom;
    numeric dy = 2 / zoom;
    if (x > y) {
      /* widescreen image */
      dx = 1.0 * x / y * dy;
    } else if (y > x) {
      /* portrait */
      dy = 1.0 * y / x * dx;
    }  // otherwise square
    return vec2{dx / x, dy / y};
  }

  static vec4 calc_bounds(size_t x, size_t y, const vec2 &center, const double &zoom) {
    numeric dx = numeric(2) / numeric(zoom);
    numeric dy = numeric(2) / numeric(zoom);
    if (x > y) {
      /* widescreen image */
      dx = 1.0 * x / y * dy;
    } else if (y > x) {
      /* portrait */
      dy = 1.0 * y / x * dx;
    }  // otherwise square
    return vec4{
        center[0] - dx, center[0] + dx, center[1] - dy, center[1] + dy,
    };
  }

  template <bool smooth, typename Func>
  static double fractal_cell_(const complex &_z, const complex &c, const size_t max_iterations) {
    const double cap = max_iterations * max_iterations;
    complex z = _z;
    for (size_t i = 0; i < max_iterations; i++) {
      z = Func::func(z, c);
      if (norm(z) > cap) {
        if (smooth) {
          return i - log2(log2(norm(z) + 1) + 1) + 4.0;
        } else {
          return i;
        }
      }
    }
    return 0.0;
  }

  static double fractal_cell(const complex &z, const complex &c, const size_t max_iter,
                             const bool smooth, const polynomial_t poly) {
    if (smooth) {
      switch (poly) {
        // clang-format off
case STANDARD:           return fractal_cell_<true,  func_standard<numeric>          >(z, c, max_iter);
case CUBIC:              return fractal_cell_<true,  func_cubic<numeric>             >(z, c, max_iter);
case QUADRATIC_RATIONAL: return fractal_cell_<true,  func_quadratic_rational<numeric>>(z, c, max_iter);
case INV_C:              return fractal_cell_<true,  func_inv_c<numeric>             >(z, c, max_iter);
case INV_C_PARABOLA:     return fractal_cell_<true,  func_inv_c_parabola<numeric>    >(z, c, max_iter);
}} else {switch (poly) {
case STANDARD:           return fractal_cell_<false, func_standard<numeric>          >(z, c, max_iter);
case CUBIC:              return fractal_cell_<false, func_cubic<numeric>             >(z, c, max_iter);
case QUADRATIC_RATIONAL: return fractal_cell_<false, func_quadratic_rational<numeric>>(z, c, max_iter);
case INV_C:              return fractal_cell_<false, func_inv_c<numeric>             >(z, c, max_iter);
case INV_C_PARABOLA:     return fractal_cell_<false, func_inv_c_parabola<numeric>    >(z, c, max_iter);
          // clang-format on
      }
    }
    // should only happen if polynomial type is some garbage values
    abort();
  }

 public:
  polynomial_t polynomial = STANDARD;
  matrix<double> iterations;
  matrix<bool> grid_mask;
  size_t max_iterations = 512;
  double pixel_width_x;
  double pixel_width_y;
  bool do_grid = false;
  bool is_julia = false;
  bool smooth = false;
  bool do_sine_transform = true;
  bool subsample = false;
  double mul = 1;
  complex c = complex(0.0, 0);

 protected:
  vec4 bounds{-2, 2, -2, 2};

 public:
  numeric iterate_cell(const complex pos) {
    // @formatter:off
    if (subsample) {
      numeric out[] = {0, 0, 0, 0};
      if (is_julia) {
        out[0] =
            fractal_cell(pos + complex(-pixel_width_x, 0), c, max_iterations, smooth, polynomial);
        out[1] =
            fractal_cell(pos + complex(pixel_width_x, 0), c, max_iterations, smooth, polynomial);
        out[2] =
            fractal_cell(pos + complex(0, -pixel_width_y), c, max_iterations, smooth, polynomial);
        out[3] =
            fractal_cell(pos + complex(0, pixel_width_y), c, max_iterations, smooth, polynomial);
      } else {
        out[0] = fractal_cell(complex(0, 0), pos + complex(-pixel_width_x, 0), max_iterations,
                              smooth, polynomial);
        out[1] = fractal_cell(complex(0, 0), pos + complex(pixel_width_x, 0), max_iterations,
                              smooth, polynomial);
        out[2] = fractal_cell(complex(0, 0), pos + complex(0, -pixel_width_y), max_iterations,
                              smooth, polynomial);
        out[3] = fractal_cell(complex(0, 0), pos + complex(0, pixel_width_y), max_iterations,
                              smooth, polynomial);
      }
      return ((out[0] + out[1]) + (out[2] + out[3])) / 4;
    } else {
      if (is_julia) {
        return fractal_cell(pos, c, max_iterations, smooth, polynomial);
      } else {
        return fractal_cell(complex(0, 0), pos, max_iterations, smooth, polynomial);
      }
    }
    // @formatter:on
  }

  bool process_line(const line &l) {
    const vec_ull start = l.start_point;
    const vec_ull end = l.end_point;
    // handle lines containing only a single pixel
    const vec_ull diff = ((start - end) != vec_ull{0, 0}) ? (end - start).unitV() : vec_ull{0, 0};
    const size_t length = (end - start).norm();
    bool out = true;

    for (size_t i = 0; i <= length; i++) {
      vec_ull pos = start + diff * i;
      if (iterations(pos) == NOT_DEFINED) {
        // imaginary axis is different because it points opposite our +y axis
        complex complex_pos = index_to_complex(pos);
        iterations(pos) = iterate_cell(complex_pos);
      }
      if (iterations(pos) != iterations(start)) {
        out = false;
      }
    }
    return out;
  }

  void set_zoom(const vec2 &center, const numeric &zoom) {
    bounds = calc_bounds(iterations.x(), iterations.y(), center, zoom);
    auto wid = calc_pixel_widths(iterations.x(), iterations.y(), zoom);
    pixel_width_x = wid[0];
    pixel_width_y = wid[1];
  }

  void set_polynomial(const std::string &name) {
    auto iter_name = names.find(name);
    if (iter_name == names.end()) {
      polynomial = STANDARD;
    } else {
      polynomial = iter_name->second;
    }
  }

  split_rectangle process_rectangle(rectangle r) {
    bool edges_equal = true;
    for (auto &side : r.get_sides()) {
      // pre-calculate to avoid lazy evaluation skipping
      bool res = process_line(side);
      edges_equal = edges_equal && res;
    }
    size_t shortest_edge = std::min(r.xmax - r.xmin, r.ymax - r.ymin);
    if (!edges_equal && shortest_edge > 1) {
      // must be careful how we round up and down because rectangles are
      // inclusive
      // on all bounds
      return {
          true,
          {
              rectangle(r.xmin, (r.xmin + r.xmax) / 2, r.ymin, (r.ymin + r.ymax) / 2),
              rectangle((r.xmin + r.xmax) / 2, r.xmax, r.ymin, (r.ymin + r.ymax) / 2),
              rectangle(r.xmin, (r.xmin + r.xmax) / 2, (r.ymin + r.ymax) / 2, r.ymax),
              rectangle((r.xmin + r.xmax) / 2, r.xmax, (r.ymin + r.ymax) / 2, r.ymax),
          },
      };
    } else if (edges_equal /*&& shortest_edge < longest_bound / 2*/) {
      numeric iter_fill = iterations(r.xmin, r.ymin);
      for (size_t i = r.xmin; i <= r.xmax; i++) {
        for (size_t j = r.ymin; j <= r.ymax; j++) {
          iterations(i, j) = iter_fill;
        }
      }
      if (do_grid) {
        for (size_t j = r.ymin; j < r.ymax; j++) {
          grid_mask(r.xmin, j) = true;
        }
        for (size_t i = r.xmin; i < r.xmax; i++) {
          grid_mask(i, r.ymin) = true;
        }
      }
    }
    return {false, {}};
  }

  fractal_base(const size_t w, const size_t h)
      : iterations(w, h, NOT_DEFINED),
        grid_mask(w, h, 0),
        pixel_width_x(2 / w),
        pixel_width_y(2 / h) {}

  fractal_base() {}

  complex index_to_complex(const vec_ull &pos) {
    return complex((pos[0] * 1.0 / iterations.x()) * (bounds[1] - bounds[0]) + bounds[0],
                   bounds[3] - (pos[1] * 1.0 / iterations.y()) * (bounds[3] - bounds[2]));
  }

  vec4 get_bounds() const { return bounds; }

  void set_bounds(vec4 bounds) { this->bounds = bounds; }

  void read_config(const fractal_info &cfg) {
    iterations = matrix<double>(cfg.x, cfg.y, NOT_DEFINED);
    grid_mask = matrix<bool>(cfg.x, cfg.y, 0);
    pixel_width_x = 2.0 / cfg.x;
    pixel_width_y = 2.0 / cfg.y;
    set_zoom(vec2{cfg.r, cfg.i}, cfg.zoom);
    set_polynomial(cfg.poly);
    c = complex(cfg.cr, cfg.ci);
    subsample = cfg.subsample;
    smooth = cfg.smooth;
    do_grid = cfg.do_grid;
    is_julia = cfg.is_julia;
    max_iterations = cfg.iter;
    mul = cfg.mul;
  }

  fractal_base(const fractal_base<numeric> &rhs)
      : iterations(rhs.iterations),
        grid_mask(rhs.grid_mask),
        pixel_width_x(rhs.pixel_width_x),
        pixel_width_y(rhs.pixel_width_y),
        polynomial(rhs.polynomial),
        max_iterations(rhs.max_iterations),
        do_grid(rhs.do_grid),
        is_julia(rhs.is_julia),
        smooth(rhs.smooth),
        do_sine_transform(rhs.do_sine_transform),
        subsample(rhs.subsample),
        mul(rhs.mul),
        c(rhs.c) {}
};
};
