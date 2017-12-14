// (c) Copyright 2017 Josh Wright
#include "fractal_animation_zoom.h"

namespace image_utils {

void fractal_animation_zoom::animation_worker::render(double t) {
  double zoom = std::exp(t * std::log(p.max_zoom));

  std::fill(fractal->iterations.begin(), fractal->iterations.end(), -1.0);
  std::fill(color_image.begin(), color_image.end(), RGB{0, 0, 0});

  fractal->set_zoom(std::to_string(p.center[0]), std::to_string(p.center[1]), std::to_string(zoom));
  fractal->run();

  log_transform(fractal->iterations);
  sine_transform(fractal->iterations);

  scale_grid(fractal->iterations);
  image_sanity_check(fractal->iterations, false);
  grayscale_to_rgb(fractal->iterations, color_image, p.cmap);
}

fractal_animation_zoom::animation_worker::animation_worker(const fractal_animation_zoom &parent)
    : worker(),
      p(parent),
      fractal(get_fractal(p.base_cfg)),
      color_image(p.base_cfg.x, p.base_cfg.y, {0, 0, 0}) {}

image_RGB &fractal_animation_zoom::animation_worker::get_color_image() { return color_image; }

worker_ref fractal_animation_zoom::get_worker() { return std::make_shared<animation_worker>(*this); }

fractal_animation_zoom::~fractal_animation_zoom() {}

fractal_animation_zoom::fractal_animation_zoom(const fractal_info &cfg) :base_cfg(cfg) {}
};
