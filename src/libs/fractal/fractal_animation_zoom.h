// (c) Copyright 2017 Josh Wright
#pragma once

#include "fractal_animator.h"
#include "fractal_common.h"

namespace image_utils {
class fractal_animation_zoom : public animation {
 public:
  fractal_info base_cfg;
  vec2 center = vec2{-0.743643887037151, -0.743643887037151};
  double max_zoom = 1e12;
  colormap cmap = read_colormap_from_string("hot");

  fractal_animation_zoom(const fractal_info &cfg);

  worker_ref get_worker() override;

  ~fractal_animation_zoom() override;

 private:
  struct animation_worker : worker {
    const fractal_animation_zoom &p;
    fractal_ref fractal;
    image_RGB color_image;
    animation_worker(const fractal_animation_zoom &parent);

    void render(double t) override;

    image_RGB &get_color_image() override;
  };
};
}
