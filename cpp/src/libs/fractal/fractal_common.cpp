// (c) Copyright 2016 Josh Wright

#include <boost/multiprecision/float128.hpp>
#include <boost/multiprecision/gmp.hpp>
using namespace boost::multiprecision;
using namespace boost::math;
using namespace boost::math::tools;

#include "fractal_common.h"
#include "fractal_impl.h"
#include "generators.h"

namespace image_utils {

complex complex_circle(const complex center, const double r, const double t) {
  complex pos(std::cos(2 * PI * t), std::sin(2 * PI * t));
  return center + r * pos;
}

void sine_transform(matrix<double> &in, const double multiplier, const double rel_phase,
                    bool preserve_zero) {
  if (preserve_zero) {
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < in.size(); i++) {
      if (in(i) != 0) {
        in(i) = sin(in(i)*PI / 4 * multiplier + 2 * PI * rel_phase);
        in(i) *= in(i);
      }
    }
  } else {
#pragma omp parallel for schedule(static)
    for (size_t i = 0; i < in.size(); i++) {
      in(i) = sin(in(i)*PI / 4 * multiplier + 2 * PI * rel_phase);
      in(i) *= in(i);
    }
  }
}

void log_transform(matrix<double> &in, const double multiplier) {
#pragma omp parallel for schedule(static)
  for (size_t i = 0; i < in.size(); i++) {
    in(i) = log2((in(i) + 1) * multiplier);
  }
}

// maximum precision available numeric type
typedef mpf_float precise_numeric;

std::string precise_mul(const std::string &_a, const std::string &_b) {
  precise_numeric a = numeric_from_string<precise_numeric>(_a);
  precise_numeric b = numeric_from_string<precise_numeric>(_b);
  std::stringstream ss;
  ss << a * b;
  return ss.str();
}

///////////////////////////////////////////////////////////////////

void fractal::read_config(const fractal_info &cfg) {
  iterations = matrix<double>(cfg.x, cfg.y, NOT_DEFINED);
  grid_mask = matrix<bool>(cfg.x, cfg.y, 0);
  subsample = cfg.subsample;
  smooth = cfg.smooth;
  do_grid = cfg.do_grid;
  is_julia = cfg.is_julia;
  max_iterations = cfg.iter;
  mul = cfg.mul;
}

template <typename numeric>
fractal_ref get_fractal_helper(const fractal_info &cfg) {
  using std::make_shared;
  fractal_ref fract;
  auto it = polynomial_names.find(cfg.poly);
  switch (it->second) {
    case STANDARD:
      fract = make_shared<fractal_impl<numeric, func_standard<numeric>>>(cfg.x, cfg.y);
      break;
    case CUBIC:
      fract = make_shared<fractal_impl<numeric, func_cubic<numeric>>>(cfg.x, cfg.y);
      break;
    case QUADRATIC_RATIONAL:
      fract = make_shared<fractal_impl<numeric, func_quadratic_rational<numeric>>>(cfg.x, cfg.y);
      break;
    case INV_C:
      fract = make_shared<fractal_impl<numeric, func_inv_c<numeric>>>(cfg.x, cfg.y);
      break;
    case INV_C_PARABOLA:
      fract = make_shared<fractal_impl<numeric, func_inv_c_parabola<numeric>>>(cfg.x, cfg.y);
      break;
  }
  fract->read_config(cfg);
  return fract;
}

fractal_ref get_fractal(const fractal_info &cfg) {
  switch (cfg.bits) {
    case 64:
      return get_fractal_helper<double>(cfg);
    case 128:
      return get_fractal_helper<float128>(cfg);
    default:
      mpf_float::default_precision(cfg.bits);
      return get_fractal_helper<mpf_float>(cfg);
  }
}

void fractal::run() { run_multithread(); }
};
