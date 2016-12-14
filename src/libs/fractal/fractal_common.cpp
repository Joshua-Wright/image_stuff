// (c) Copyright 2016 Josh Wright
#include "fractal_common.h"
#include "generators.h"

namespace image_utils {

    complex complex_circle(const complex center, const double r, const double t) {
        complex pos(std::cos(2 * PI * t), std::sin(2 * PI * t));
        return center + r * pos;
    }

    double fractal_cell(complex z, const complex &c, const size_t m, const bool s,
                        const polynomial_t poly, mandelbrot_polynomial_t custom) {
        switch (poly) {
            default:
            case STANDARD:
                return fractal_cell_(z, c, m, s, func_standard);
            case CUBIC:
                return fractal_cell_(z, c, m, s, func_cubic);
            case INV_C:
                return fractal_cell_(z, c, m, s, func_inv_c);
            case LAMBDA:
                return fractal_cell_(z, c, m, s, func_lambda);
            case INV_LAMBDA:
                return fractal_cell_(z, c, m, s, func_inv_lambda);
            case INV_C_PARABOLA:
                return fractal_cell_(z, c, m, s, func_inv_c_parabola);
            case QUADRATIC_RATIONAL:
                return fractal_cell_(z, c, m, s, func_quadratic_rational);
            case CUSTOM:
                return fractal_cell_(z, c, m, s, custom);
        }
    }

    void sine_transform(matrix<double> &in, const double multiplier, const double rel_phase, bool preserve_zero) {
        if (preserve_zero) {
#pragma omp parallel for schedule(static)
            for (size_t i = 0; i < in.size(); i++) {
                if (in(i) != 0) {
                    in(i) = sin(in(i) * PI / 4 * multiplier + 2 * PI * rel_phase);
                    in(i) *= in(i);
                }
            }
        } else {
#pragma omp parallel for schedule(static)
            for (size_t i = 0; i < in.size(); i++) {
                in(i) = sin(in(i) * PI / 4 * multiplier + 2 * PI * rel_phase);
                in(i) *= in(i);
            }
        }
    }

    void log_transform(matrix<double> &in, const double multiplier) {
#pragma omp parallel for schedule(static)
        for (size_t i = 0; i < in.size(); i++) {
            in(i) = log2((in(i) + 1) * multiplier);
        }
    }

    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////

    double fractal_base::iterate_cell(const complex pos) {
        // @formatter:off
        if (subsample) {
            double out[] = {0, 0, 0, 0};
            if (is_julia) {
                out[0] = fractal_cell(pos + complex(-pixel_width_x, 0), c, max_iterations, smooth, polynomial, custom_polynomial);
                out[1] = fractal_cell(pos + complex(pixel_width_x, 0), c, max_iterations, smooth, polynomial, custom_polynomial);
                out[2] = fractal_cell(pos + complex(0, -pixel_width_y), c, max_iterations, smooth, polynomial, custom_polynomial);
                out[3] = fractal_cell(pos + complex(0, pixel_width_y), c, max_iterations, smooth, polynomial, custom_polynomial);
            } else {
                out[0] = fractal_cell(complex(0, 0), pos + complex(-pixel_width_x, 0), max_iterations, smooth, polynomial, custom_polynomial);
                out[1] = fractal_cell(complex(0, 0), pos + complex(pixel_width_x, 0), max_iterations, smooth, polynomial, custom_polynomial);
                out[2] = fractal_cell(complex(0, 0), pos + complex(0, -pixel_width_y), max_iterations, smooth, polynomial, custom_polynomial);
                out[3] = fractal_cell(complex(0, 0), pos + complex(0, pixel_width_y), max_iterations, smooth, polynomial, custom_polynomial);
            }
            return ((out[0] + out[1]) + (out[2] + out[3])) / 4;
        } else {
            if (is_julia) {
                return fractal_cell(pos, c, max_iterations, smooth, polynomial, custom_polynomial);
            } else {
                return fractal_cell(complex(0, 0), pos, max_iterations, smooth, polynomial, custom_polynomial);
            }
        }
        // @formatter:on
    }

    bool fractal_base::process_line(const line &l) {
        const vec_ull start = l.start_point;
        const vec_ull end = l.end_point;
        // handle lines containing only a single pixel
        const vec_ull diff = ((start - end) != vec_ull{0, 0}) ? (end - start).unitV() : vec_ull{0, 0};
        const size_t length = (end - start).norm();
        bool out = true;

        for (size_t i = 0; i <= length; i++) {
            vec_ull pos = start + diff * i;
            if (iterations(pos) == NOT_DEFINED) {
                // imaginary axis is different because it points opposite our +y axis
                complex complex_pos = index_to_complex(pos);
                iterations(pos) = iterate_cell(complex_pos);
            }
            if (iterations(pos) != iterations(start)) {
                out = false;
            }
        }
        return out;
    }

    void fractal_base::set_zoom(vec2 center, double zoom) {
        double dx = 2 / zoom;
        double dy = 2 / zoom;
        if (iterations.x() > iterations.y()) {
            /* widescreen image */
            dx = 1.0 * iterations.x() / iterations.y() * dy;
        } else if (iterations.y() > iterations.x()) {
            /* portrait */
            dy = 1.0 * iterations.y() / iterations.x() * dx;
        } // otherwise square
        bounds = vec4({
                              center[0] - dx,
                              center[0] + dx,
                              center[1] - dy,
                              center[1] + dy,
                      });
        pixel_width_x = dx / iterations.x();
        pixel_width_y = dy / iterations.y();
    }

    void fractal_base::set_polynomial(const std::string &name) {
        auto iter_name = names.find(name);
        if (iter_name == names.end()) {
            polynomial = STANDARD;
        } else {
            polynomial = iter_name->second;
        }
    }

    split_rectangle fractal_base::process_rectangle(rectangle r) {
        bool edges_equal = true;
        for (auto &side : r.get_sides()) {
            // pre-calculate to avoid lazy evaluation skipping
            bool res = process_line(side);
            edges_equal = edges_equal && res;
        }
        size_t shortest_edge = std::min(r.xmax - r.xmin, r.ymax - r.ymin);
        if (!edges_equal && shortest_edge > 1) {
            // must be careful how we round up and down because rectangles are inclusive on all bounds
            return {true,
                    {
                            rectangle(r.xmin, (r.xmin + r.xmax) / 2, r.ymin, (r.ymin + r.ymax) / 2),
                            rectangle((r.xmin + r.xmax) / 2, r.xmax, r.ymin, (r.ymin + r.ymax) / 2),
                            rectangle(r.xmin, (r.xmin + r.xmax) / 2, (r.ymin + r.ymax) / 2, r.ymax),
                            rectangle((r.xmin + r.xmax) / 2, r.xmax, (r.ymin + r.ymax) / 2, r.ymax),
                    },
            };
        } else if (edges_equal /*&& shortest_edge < longest_bound / 2*/) {
            double iter_fill = iterations(r.xmin, r.ymin);
            for (size_t i = r.xmin; i <= r.xmax; i++) {
                for (size_t j = r.ymin; j <= r.ymax; j++) {
                    iterations(i, j) = iter_fill;
                }
            }
            if (do_grid) {
                for (size_t j = r.ymin; j < r.ymax; j++) {
                    grid_mask(r.xmin, j) = true;
                }
                for (size_t i = r.xmin; i < r.xmax; i++) {
                    grid_mask(i, r.ymin) = true;
                }
            }
        }
        return {false, {}};
    }

    fractal_base::fractal_base(const size_t w, const size_t h) : iterations(w, h, NOT_DEFINED),
                                                                 grid_mask(w, h, 0),
                                                                 pixel_width_x(2 / w),
                                                                 pixel_width_y(2 / h) {}

    fractal_base::fractal_base() {}

    complex fractal_base::index_to_complex(const vec_ull &pos) {
        return complex(
                (pos[0] * 1.0 / iterations.x()) * (bounds[1] - bounds[0]) + bounds[0],
                bounds[3] - (pos[1] * 1.0 / iterations.y()) * (bounds[3] - bounds[2])
        );
    }

    fractal_base::fractal_base(const fractal_base &rhs)
            : iterations(rhs.iterations),
              grid_mask(rhs.grid_mask),
              pixel_width_x(rhs.pixel_width_x),
              pixel_width_y(rhs.pixel_width_y),
              custom_polynomial(rhs.custom_polynomial),
              polynomial(rhs.polynomial),
              max_iterations(rhs.max_iterations),
              do_grid(rhs.do_grid),
              is_julia(rhs.is_julia),
              smooth(rhs.smooth),
              do_sine_transform(rhs.do_sine_transform),
              subsample(rhs.subsample),
              mul(rhs.mul),
              c(rhs.c) {}

};

