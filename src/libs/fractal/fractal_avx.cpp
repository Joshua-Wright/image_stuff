#include "fractal_avx.h"
#include <immintrin.h>

namespace image_utils {

// TODO factor out to shared method
static vec2 calc_pixel_widths(size_t x, size_t y, const double &zoom) {
  double dx = double(2) / zoom;
  double dy = double(2) / zoom;
  if (x > y) {
    /* widescreen image */
    dx = double(1.0) * x / y * dy;
  } else if (y > x) {
    /* portrait */
    dy = double(1.0) * y / x * dx;
  }  // otherwise square
  return vec2{dx / x, dy / y};
}

// TODO refactor out to non-class method
static vec4 calc_bounds(size_t x, size_t y, const vec2 &center, const double &zoom) {
  double dx = double(2.0) / double(zoom);
  double dy = double(2.0) / double(zoom);
  if (x > y) {
    /* widescreen image */
    dx = double(1.0) * x / y * dy;
  } else if (y > x) {
    /* portrait */
    dy = double(1.0) * y / x * dx;
  }  // otherwise square
  return vec4{
      center[0] - dx, center[0] + dx, center[1] - dy, center[1] + dy,
  };
}

complex fractal_avx_f32::index_to_complex(const vec_ull &pos) {
  return complex((pos[0] * 1.0 / iterations.x()) * (bounds[1] - bounds[0]) + bounds[0],
                 bounds[3] - (pos[1] * 1.0 / iterations.y()) * (bounds[3] - bounds[2]));
}

void fractal_avx_f32::set_zoom(const vec2 &center, const double &zoom) {
  bounds = calc_bounds(iterations.x(), iterations.y(), center, zoom);
  auto wid = calc_pixel_widths(iterations.x(), iterations.y(), zoom);
  pixel_width_x = wid[0];
  pixel_width_y = wid[1];
}

void fractal_avx_f32::read_config(const fractal_info &cfg) {
  fractal::read_config(cfg);
  pixel_width_x = 2.0 / double(cfg.x);
  pixel_width_y = 2.0 / double(cfg.y);
  set_zoom(cfg.r, cfg.i, cfg.zoom);
  c = complex(numeric_from_string<double>(cfg.cr), numeric_from_string<double>(cfg.ci));
}

////////////////////////////////////////////////////////////////////////

void fractal_avx_f32::run_singlethread() {
  // TODO real single thread method
  run_multithread();
}
void fractal_avx_f32::run_multithread() {
  run_avx_f32();
  log_transform(iterations);
  sine_transform(iterations, mul);
};

void fractal_avx_f32::run_avx_f32() {
  size_t width = iterations.x();
  size_t height = iterations.y();
  __m256 xmin = _mm256_set1_ps(bounds[0]);
  __m256 ymin = _mm256_set1_ps(bounds[2]);
  __m256 xscale = _mm256_set1_ps((bounds[1] - bounds[0]) / width);
  __m256 yscale = _mm256_set1_ps((bounds[3] - bounds[2]) / height);
  // __m256 threshold = _mm256_set1_ps(4);
  __m256 threshold = _mm256_set1_ps(max_iterations * max_iterations);
  __m256 one = _mm256_set1_ps(1);
  // __m256 iter_scale = _mm256_set1_ps(1.0f / max_iterations);
  // __m256 depth_scale = _mm256_set1_ps(2 - 1);

  #pragma omp parallel for schedule(dynamic, 1)
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x += 8) {
      __m256 mx = _mm256_set_ps(x + 7, x + 6, x + 5, x + 4, x + 3, x + 2, x + 1, x + 0);
      __m256 my = _mm256_set1_ps(y);
      __m256 cr = _mm256_add_ps(_mm256_mul_ps(mx, xscale), xmin);
      __m256 ci = _mm256_add_ps(_mm256_mul_ps(my, yscale), ymin);
      __m256 zr = cr;
      __m256 zi = ci;
      int k = 1;
      __m256 mk = _mm256_set1_ps(k);
      while (++k < (int)max_iterations) {
        /* Compute z1 from z0 */
        __m256 zr2 = _mm256_mul_ps(zr, zr);
        __m256 zi2 = _mm256_mul_ps(zi, zi);
        __m256 zrzi = _mm256_mul_ps(zr, zi);
        /* zr1 = zr0 * zr0 - zi0 * zi0 + cr */
        /* zi1 = zr0 * zi0 + zr0 * zi0 + ci */
        zr = _mm256_add_ps(_mm256_sub_ps(zr2, zi2), cr);
        zi = _mm256_add_ps(_mm256_add_ps(zrzi, zrzi), ci);

        /* Increment k */
        zr2 = _mm256_mul_ps(zr, zr);
        zi2 = _mm256_mul_ps(zi, zi);
        __m256 mag2 = _mm256_add_ps(zr2, zi2);
        __m256 mask = _mm256_cmp_ps(mag2, threshold, _CMP_LT_OS);
        mk = _mm256_add_ps(_mm256_and_ps(mask, one), mk);

        /* Early bailout? */
        if (_mm256_testz_ps(mask, _mm256_set1_ps(-1))) {
          break;
        }
      }
      // mk = _mm256_mul_ps(mk, iter_scale);
      // mk = _mm256_sqrt_ps(mk);
      // mk = _mm256_mul_ps(mk, depth_scale);

      // __m256i pixels = _mm256_cvtps_epi32(mk);
      // int *src = (int *)&pixels;

      float *src = (float *)&mk;

      for (int i = 0; i < 8; i++) {
        iterations(x + i, y) = src[i];
        // iterations(x, y+i) = src[i];
      }
    }
  }
}

////////////////////////////////////////////////////////////////

fractal_avx_f32::fractal_avx_f32(const size_t w, const size_t h) : fractal(w, h) {}

fractal_avx_f32::fractal_avx_f32(const fractal_avx_f32 &rhs)
    : fractal(rhs), pixel_width_x(rhs.pixel_width_x), pixel_width_y(rhs.pixel_width_y), c(rhs.c) {}

fractal_ref get_fractal_avx_f32(const fractal_info &cfg) {
  fractal_ref ref = std::make_shared<fractal_avx_f32>(cfg.x, cfg.y);
  ref->read_config(cfg);
  return ref;
}
};
