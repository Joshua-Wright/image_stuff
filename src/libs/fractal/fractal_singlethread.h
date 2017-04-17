// (c) Copyright 2016 Josh Wright
#ifndef IMAGE_STUFF_FRACTAL_SINGLETHREAD_H
#define IMAGE_STUFF_FRACTAL_SINGLETHREAD_H

#include <map>
#include "fractal_common.h"
#include "types.h"

namespace image_utils {

template <typename numeric = double>
class fractal_singlethread : public fractal_base<numeric> {
  // class-scope variable so it's only allocated once
  rectangle_stack stack = rectangle_stack(256);
  typedef fractal_base<numeric> base;

 public:
  fractal_singlethread(const size_t w, const size_t h) : base(w, h) {}

  void run() {
    std::fill(base::iterations.begin(), base::iterations.end(), NOT_DEFINED);
    base::pixel_width_x = 2 / base::iterations.x();
    base::pixel_width_y = 2 / base::iterations.y();
    stack.push_back(rectangle(0, base::iterations.x() / 2, 0, base::iterations.y() / 2));
    stack.push_back(
        rectangle(base::iterations.x() / 2, base::iterations.x() - 1, 0, base::iterations.y() / 2));
    stack.push_back(
        rectangle(0, base::iterations.x() / 2, base::iterations.y() / 2, base::iterations.y() - 1));
    stack.push_back(rectangle(base::iterations.x() / 2, base::iterations.x() - 1,
                              base::iterations.y() / 2, base::iterations.y() - 1));

    while (!stack.empty()) {
      rectangle current = stack.pop();
      split_rectangle sp = base::process_rectangle(current);
      if (sp.did_split) {
        stack.push(sp.rectangles[0]);
        stack.push(sp.rectangles[1]);
        stack.push(sp.rectangles[2]);
        stack.push(sp.rectangles[3]);
      }
    }
    if (base::do_sine_transform) {
      log_transform(base::iterations);
      sine_transform(base::iterations, base::mul);
    }
  }

  fractal_singlethread() {}

  fractal_singlethread(const fractal_singlethread &rhs)
      // stack should always be empty when not running, so actually copying it is probably overkill
      : base(rhs),
        stack(rhs.stack) {}
};
}

#endif  // IMAGE_STUFF_FRACTAL_SINGLETHREAD_H
