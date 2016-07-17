//
// Created by j0sh on 5/27/16.
//
#include <complex>
#include <cmath>
#include <stack>
#include <boost/lockfree/stack.hpp>
#include <thread>
#include <vector>
#include "fractal.h"
#include "util/debug.h"
#include "util/vect.h"
#include "types.h"

#ifndef DO_GRID
#define DO_GRID 1
#endif


using std::cerr;
using std::endl;

namespace image_utils {
    const size_t NOT_DEFINED = (const size_t) -1;

    struct line {
        vect_ull start_point;
        vect_ull end_point;
    };

    struct rectangle {
        // bounds are inclusive
        size_t xmin, xmax, ymin, ymax;
        vect_ull corners[4];
        line sides[4];

        rectangle() { }

        rectangle(const size_t x_min, const size_t x_max,
                  const size_t y_min, const size_t y_max) : xmin(x_min), xmax(x_max),
                                                            ymin(y_min), ymax(y_max) {
            corners[0] = {x_min, y_min};
            corners[1] = {x_max, y_min};
            corners[2] = {x_min, y_max};
            corners[3] = {x_max, y_max};

            sides[0] = {corners[0], corners[1]};
            sides[1] = {corners[0], corners[2]};
            sides[2] = {corners[1], corners[3]};
            sides[3] = {corners[2], corners[3]};
        };
    };

    class fractal {
        matrix<size_t> indexes;
        const size_t iterations;
        const std::array<double, 4> bounds;
        bool do_grid;
        bool is_julia;
        complex c;

        size_t iterate_cell(const size_t iterations, const complex pos) {
            complex val = pos;
            if (is_julia) {
                for (size_t iter = 0; iter < iterations; ++iter) {
                    if (std::abs(val) < 4) {
                        val = pow(val, 2) + c;
                    } else {
                        return iter;
                    }
                }
            } else {
                for (size_t iter = 0; iter < iterations; ++iter) {
                    if (std::abs(val) < 4) {
                        val = pow(val, 2) + pos;
                    } else {
                        return iter;
                    }
                }
            }
            return 0;
        }

        bool process_line(const line &l) {
            vect_ull start = l.start_point;
            vect_ull end = l.end_point;
            vect_ull diff;
            // handle lines containing only a single pixel
            if ((start - end) != vect_ull{0, 0}) {
                diff = (end - start).unitV();
            } else {
                diff = vect{0, 0};
            }
            bool all_equal = true;
            complex start_complex(
                    (start[0] / indexes.x()) * (bounds[1] - bounds[0]) + bounds[0],
                    (start[1] / indexes.y()) * (bounds[3] - bounds[2]) + bounds[2]
            );
            size_t first_iter = iterate_cell(iterations, start_complex);
            for (size_t i = 0; i <= (end - start).norm(); i++) {
                vect_ull pos = start + diff * i;
                if (indexes((const size_t) pos[0], (const size_t) pos[1]) == NOT_DEFINED) {
                    complex complex_pos;
                    complex_pos.real((pos[0] * 1.0 / indexes.x()) * (bounds[1] - bounds[0]) + bounds[0]);
                    complex_pos.imag((pos[1] * 1.0 / indexes.y()) * (bounds[3] - bounds[2]) + bounds[2]);
                    size_t iter = iterate_cell(iterations, complex_pos);
                    indexes((const size_t) pos[0], (const size_t) pos[1]) = iter;
                }
                all_equal &= (first_iter == indexes((const size_t) pos[0], (const size_t) pos[1]));
            }
            return all_equal;
        }


        void process_rectangle(rectangle r) {
            bool edges_equal = true;
            for (auto &line : r.sides) {
                // pre-calculate to avoid lazy evaluation skipping
                bool res = process_line(line);
                edges_equal = edges_equal && res;
            }
            size_t shortest_edge = std::min(r.xmax - r.xmin, r.ymax - r.ymin);
            size_t longest_bound = std::max(indexes.x(), indexes.y());
            // todo improve magic number?
            if (edges_equal && shortest_edge < longest_bound / 20) {
                size_t iter_fill = indexes(r.xmin, r.ymin);
                for (size_t i = r.xmin; i < r.xmax; i++) {
                    for (size_t j = r.ymin; j < r.ymax; j++) {
                        indexes(i, j) = iter_fill;
                    }
                    if (do_grid) {
                        indexes(i, r.ymin) = 100;
                    }
                }
                if (do_grid) {
                    for (size_t j = r.ymin; j < r.ymax; j++) {
                        indexes(r.xmin, j) = 100;
                    }
                }
            } else if (shortest_edge > 1) {
                // must be careful how we round up and down because rectangles are inclusive on all bounds
                process_rectangle(rectangle(r.xmin, (r.xmin + r.xmax) / 2, r.ymin, (r.ymin + r.ymax) / 2));
                process_rectangle(rectangle((r.xmin + r.xmax) / 2, r.xmax, r.ymin, (r.ymin + r.ymax) / 2));
                process_rectangle(rectangle(r.xmin, (r.xmin + r.xmax) / 2, (r.ymin + r.ymax) / 2, r.ymax));
                process_rectangle(rectangle((r.xmin + r.xmax) / 2, r.xmax, (r.ymin + r.ymax) / 2, r.ymax));
            }
        }

    public:
        fractal(const size_t w, const size_t h, const size_t iter, const std::array<double, 4> bounds, bool do_grid = false)
                : indexes(w, h, NOT_DEFINED), iterations(iter), bounds(bounds), do_grid(do_grid), is_julia(false), c(0, 0) { }

        fractal(const size_t w, const size_t h, const size_t iter, const std::array<double, 4> bounds, complex c, bool do_grid = false)
                : indexes(w, h, NOT_DEFINED), iterations(iter), bounds(bounds), do_grid(do_grid), is_julia(true), c(c) { }

        matrix<double> run() {
            matrix<double> grid(indexes.x(), indexes.y());
            rectangle starter(0, indexes.x() - 1, 0, indexes.y() - 1);
            this->process_rectangle(starter);
            for (size_t i = 0; i < grid.x(); ++i) {
                for (size_t j = 0; j < grid.y(); ++j) {
                    grid(i, j) = indexes(i, j);
                }
            }
            return grid;
        }

    };

    matrix<double> fast_mandelbrot(const size_t w, const size_t h, const size_t iter, const std::array<double, 4> bounds) {
        return fractal(w, h, iter, bounds).run();
    }

    matrix<double> fast_julia(const size_t w, const size_t h, const size_t iter, const std::array<double, 4> bounds, complex c) {
        return fractal(w, h, iter, bounds, c).run();
    }

    complex complex_circle(const complex center, const double r, const double t) {
        complex pos(std::cos(2 * PI * t), std::sin(2 * PI * t));
        return center + r * pos;
    }


}