#pragma once

#include "fractal_common.h"

#pragma once
#include "fractal_common.h"
#include "util/debug.h"

namespace image_utils {

fractal_ref get_fractal_avx_f32(const fractal_info &cfg);

class fractal_avx_f32 : public fractal {
 public:
 public:
  double pixel_width_x;
  double pixel_width_y;
  complex c = complex(0.0, 0.0);

 protected:
  // vec4 bounds{-2, 2, -2, 2};
  vec4 bounds;

 public:
  complex index_to_complex(const vec_ull &pos);

  // vec4 get_bounds() const { return bounds; }
  // void set_bounds(vec4 bounds) { this->bounds = bounds; }

  /////////////////////////////////////////////////////////////////////////////

  virtual void run_singlethread();
  virtual void run_multithread();
  void run_avx_f32();

  /////////////////////////////////////////////////////////////////////////////

  fractal_avx_f32(const size_t w, const size_t h);

  fractal_avx_f32(const fractal_avx_f32 &rhs);

  virtual void read_config(const fractal_info &cfg);

  virtual void set_zoom(const std::string &r_str, const std::string &i_str,
                        const std::string &zoom_str) {
    set_zoom(vec2{numeric_from_string<double>(r_str), numeric_from_string<double>(i_str)},
             numeric_from_string<double>(zoom_str));
  }

  void set_zoom(const vec2 &center, const double &zoom);
};
};
