// (c) Copyright 2016 Josh Wright
#include "fractal_common.h"
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
};
