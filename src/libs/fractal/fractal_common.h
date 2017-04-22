// (c) Copyright 2016 Josh Wright
#pragma once

#include <util/debug.h>
#include <functional>
#include <map>
#include <memory>
#include "fractal_info.h"
#include "types.h"

namespace image_utils {
const double NOT_DEFINED = -1.0;

#define FRACTAL_POLYNOMIAL(name, expr)                                 \
  template <typename numeric>                                          \
  struct name {                                                        \
    typedef std::complex<numeric> complex;                             \
    std::complex<numeric> operator()(const std::complex<numeric> &z,   \
                                     const std::complex<numeric> &c) { \
      return expr;                                                     \
    }                                                                  \
  };

FRACTAL_POLYNOMIAL(func_standard, z *z + c)
FRACTAL_POLYNOMIAL(func_cubic, z *z *z + c)
FRACTAL_POLYNOMIAL(func_quadratic_rational, (z * z) + (c * c) / ((c * c * c * c) - numeric(0.25)))
FRACTAL_POLYNOMIAL(func_inv_c, (z * z) + numeric(1.0) / (c - numeric(1)))
FRACTAL_POLYNOMIAL(func_inv_c_parabola, (z * z) + numeric(1.0) / c + numeric(0.25))

enum polynomial_t {
  STANDARD,
  CUBIC,
  QUADRATIC_RATIONAL,
  INV_C,
  INV_C_PARABOLA,
};
const std::map<std::string, polynomial_t> polynomial_names{
    {"standard", STANDARD},
    {"cubic", CUBIC},
    {"quadratic-rational", QUADRATIC_RATIONAL},
    {"inv-c", INV_C},
    {"inv-c-parabola", INV_C_PARABOLA},
};

void sine_transform(matrix<double> &in, const double multiplier = 1, const double rel_phase = 0,
                    bool preserve_zero = true);
void log_transform(matrix<double> &in, const double multiplier = 1);

/** t on range [0,1]*/
complex complex_circle(const complex center, const double r, const double t);

template <typename numeric>
numeric numeric_from_string(const std::string &s) {
  std::stringstream ss(s);
  numeric n;
  ss >> n;
  return n;
}

/**
 * obviously not an efficient abstraction, do not use in critical loops
 */
std::string precise_mul(const std::string &a, const std::string &b);

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
//////////////////////////////////////////////////////////////////

class fractal {
 public:
  matrix<double> iterations;
  matrix<bool> grid_mask;
  size_t max_iterations = 512;
  bool do_grid = false;
  bool is_julia = false;
  bool smooth = false;
  bool do_sine_transform = true;
  bool subsample = false;
  double mul = 1;

  void run();
  virtual void run_singlethread() = 0;
  virtual void run_multithread() = 0;

  virtual void read_config(const fractal_info &cfg);

  /**
   * use strings instead of numeric types for arbitrary precision
   */
  virtual void set_zoom(const std::string &r, const std::string &i,const std::string &zoom) = 0;

  fractal();
  fractal(const size_t w, const size_t h) : iterations(w, h, NOT_DEFINED), grid_mask(w, h, 0) {}
  fractal(const fractal &rhs)
      : iterations(rhs.iterations),
        grid_mask(rhs.grid_mask),
        max_iterations(rhs.max_iterations),
        do_grid(rhs.do_grid),
        is_julia(rhs.is_julia),
        smooth(rhs.smooth),
        do_sine_transform(rhs.do_sine_transform),
        subsample(rhs.subsample),
        mul(rhs.mul) {}
};

typedef std::shared_ptr<fractal> fractal_ref;

fractal_ref get_fractal(const fractal_info &cfg);
};
