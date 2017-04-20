//
// Created by j0sh on 5/27/16.
//

#ifndef IMAGE_STUFF_MANDELBROT_H
#define IMAGE_STUFF_MANDELBROT_H

#include <map>
#include "fractal_common.h"
#include "types.h"

namespace image_utils {

template <typename numeric = double>
class fractal_multithread : public fractal_impl<numeric> {
  typedef fractal_impl<numeric> base;

 public:

  void run() {
    base::run_multithread();
  }

  fractal_multithread(const size_t w, const size_t h) : base(w, h) {}

  fractal_multithread() {}

  fractal_multithread(const fractal_multithread &rhs) : base(rhs){};
};
}
#endif  // IMAGE_STUFF_MANDELBROT_H
