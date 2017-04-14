#include "HSV.h"

namespace image_utils {
// http://stackoverflow.com/questions/3018313/algorithm-to-convert--rgb-to-hsv-and-hsv-to-vec3-in-range-0-255-for-both
HSV RGB2HSV(vec3 in) {
  in /= 255;
  HSV out;
  double min, max, delta;

  min = in[0] < in[1] ? in[0] : in[1];
  min = min < in[2] ? min : in[2];

  max = in[0] > in[1] ? in[0] : in[1];
  max = max > in[2] ? max : in[2];

  out.v = max; // v
  delta = max - min;
  if (delta < 0.00001) {
    out.s = 0;
    out.h = 0; // undefined, maybe nan?
    return out;
  }
  if (max > 0.0) {         // NOTE: if Max is == 0, this divide would cause a crash
    out.s = (delta / max); // s
  } else {
    // if max is 0, then r = g = b = 0
    // s = 0, v is undefined
    out.s = 0.0;
    out.h = NAN; // its now undefined
    return out;
  }
  if (in[0] >= max)                  // > is bogus, just keeps compilor happy
    out.h = (in[1] - in[2]) / delta; // between yellow & magenta
  else if (in[1] >= max)
    out.h = 2.0 + (in[2] - in[0]) / delta; // between cyan & yellow
  else
    out.h = 4.0 + (in[0] - in[1]) / delta; // between magenta & cyan

  out.h *= 60.0; // degrees

  if (out.h < 0.0)
    out.h += 360.0;

  return out;
}


vec3 HSV2RGB(HSV in) {
  double hh, p, q, t, ff;
  long i;
  vec3 out;

  if (in.s <= 0.0) { // < is bogus, just shuts up warnings
    out[0] = in.v;
    out[1] = in.v;
    out[2] = in.v;
    return out;
  }
  hh = in.h;
  if (hh >= 360.0)
    hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = in.v * (1.0 - in.s);
  q = in.v * (1.0 - (in.s * ff));
  t = in.v * (1.0 - (in.s * (1.0 - ff)));

  switch (i) {
  case 0:
    out[0] = in.v;
    out[1] = t;
    out[2] = p;
    break;
  case 1:
    out[0] = q;
    out[1] = in.v;
    out[2] = p;
    break;
  case 2:
    out[0] = p;
    out[1] = in.v;
    out[2] = t;
    break;

  case 3:
    out[0] = p;
    out[1] = q;
    out[2] = in.v;
    break;
  case 4:
    out[0] = t;
    out[1] = p;
    out[2] = in.v;
    break;
  case 5:
  default:
    out[0] = in.v;
    out[1] = p;
    out[2] = q;
    break;
  }
  return out * 256;
}
};
