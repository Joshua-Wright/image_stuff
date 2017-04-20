// (c) Copyright 2016 Josh Wright
#ifndef IMAGE_STUFF_FRACTAL_SINGLETHREAD_H
#define IMAGE_STUFF_FRACTAL_SINGLETHREAD_H

#include <map>
#include "fractal_common.h"
#include "types.h"

namespace image_utils {

template <typename numeric = double>
class fractal_singlethread : public fractal_impl<numeric> {
  // class-scope variable so it's only allocated once
  // rectangle_stack stack = rectangle_stack(256);
  typedef fractal_impl<numeric> base;

 public:
  fractal_singlethread(const size_t w, const size_t h) : base(w, h) {}

  void run() {
    base::run_singlethread();
  }
  fractal_singlethread() {}

  fractal_singlethread(const fractal_singlethread &rhs)
      // stack should always be empty when not running, so actually copying it is probably overkill
      : base(rhs) {}
  // : base(rhs),
  //   stack(rhs.stack) {}
};
}

#endif  // IMAGE_STUFF_FRACTAL_SINGLETHREAD_H
