// (c) Copyright 2016 Josh Wright

#include "types.h"

namespace image_utils {

vec3 RGB_to_vec3(const RGB c) {
  return vec3{
      (double)c.r, (double)c.g, (double)c.b,
  };
}

RGB vec3_to_RGB(const vec3 &v) {
  return RGB{
      clamp<unsigned char, double>(v[0], 0, 255),
      clamp<unsigned char, double>(v[1], 0, 255),
      clamp<unsigned char, double>(v[2], 0, 255),
  };
}

matrix<RGB> image_vec3_to_RGB(const matrix<vec3> &m) {
  matrix<RGB> out_rgb(m.x(), m.y());
  std::transform(m.begin(), m.end(), out_rgb.begin(), [](const vec3 &in) {
    RGB out;
    out.r = in[0];
    out.g = in[1];
    out.b = in[2];
    return out;
  });
  return out_rgb;
}

matrix<vec3> image_RGB_to_vec3(const matrix<RGB> &m) {
  matrix<vec3> out_vec3(m.x(), m.y());
  std::transform(m.begin(), m.end(), out_vec3.begin(), [](const RGB &in) {
    vec3 out;
    out[0] = in.r;
    out[1] = in.g;
    out[2] = in.b;
    return out;
  });
  return out_vec3;
}
}
