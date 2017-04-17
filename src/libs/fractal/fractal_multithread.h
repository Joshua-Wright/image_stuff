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
class fractal_multithread : public fractal_base<numeric> {
  typedef fractal_base<numeric> base;

 public:
  void run() {
    const size_t stack_size = base::iterations.x() * base::iterations.y();

    std::vector<rectangle> rectange_stack;
    // four quadrants for starting
    rectange_stack.push_back(rectangle(0, base::iterations.x() / 2, 0, base::iterations.y() / 2));
    rectange_stack.push_back(
        rectangle(base::iterations.x() / 2, base::iterations.x() - 1, 0, base::iterations.y() / 2));
    rectange_stack.push_back(
        rectangle(0, base::iterations.x() / 2, base::iterations.y() / 2, base::iterations.y() - 1));
    rectange_stack.push_back(rectangle(base::iterations.x() / 2, base::iterations.x() - 1,
                                       base::iterations.y() / 2, base::iterations.y() - 1));

    rectange_stack.reserve(stack_size);
    while (!rectange_stack.empty()) {
      std::vector<rectangle> new_stack;
      new_stack.reserve(stack_size);
#pragma omp parallel for schedule(dynamic)
      for (size_t i = 0; i < rectange_stack.size(); i++) {
        auto v = base::process_rectangle(rectange_stack[i]);

        if (v.did_split) {
#pragma omp critical
          {
            new_stack.push_back(v.rectangles[0]);
            new_stack.push_back(v.rectangles[1]);
            new_stack.push_back(v.rectangles[2]);
            new_stack.push_back(v.rectangles[3]);
          }
        }
      }
      rectange_stack = new_stack;
    }

    if (base::do_sine_transform) {
      base::iterations = base::iterations;
      log_transform(base::iterations);
      sine_transform(base::iterations, base::mul);
    }

    if (base::do_grid) {
      /*get max iteration (that was used) and use that*/
      double grid_color = *std::max_element(base::iterations.begin(), base::iterations.end());
#pragma omp parallel for schedule(static) collapse(2)
      for (size_t i = 0; i < base::iterations.x(); ++i) {
        for (size_t j = 0; j < base::iterations.y(); ++j) {
          if (base::grid_mask(i, j)) {
            base::iterations(i, j) = grid_color;
          }
        }
      }
    }
  }

  fractal_multithread(const size_t w, const size_t h) : base(w, h) {}

  fractal_multithread() {}

  fractal_multithread(const fractal_multithread &rhs) : base(rhs){};
};
}
#endif  // IMAGE_STUFF_MANDELBROT_H
