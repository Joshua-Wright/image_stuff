//
// Created by j0sh on 5/27/16.
//
#include <complex>
#include <cmath>
#include <stack>
#include <boost/lockfree/stack.hpp>
#include <thread>
#include <vector>
#include "mandelbrot.h"
#include "debug.h"
#include "vect.h"
#include "types.h"

using std::cerr;
using std::endl;
using boost::lockfree::stack;

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

    size_t iterate_cell(const size_t iterations, const std::complex<double> pos) {
        std::complex<double> val = pos;
        for (size_t iter = 0; iter < iterations; ++iter) {
            if (std::abs(val) < 4) {
                val = pow(val, 2) + pos;
            } else {
                return iter;
            }
        }
        return 0;
    }

    bool process_line(matrix<size_t> &grid_iter, const size_t iterations, line l, const std::array<double, 4> bounds) {
        vect start = l.start_point;
        vect end = l.end_point;
        vect diff;
        // handle lines containing only a single pixel
        if ((start - end) != vect{0, 0}) {
            diff = (end - start).unitV();
        } else {
            diff = vect{0, 0};
        }
        bool all_equal = true;
        std::complex<double> start_complex(
                (start[0] / grid_iter.x()) * (bounds[1] - bounds[0]) + bounds[0],
                (start[1] / grid_iter.y()) * (bounds[3] - bounds[2]) + bounds[2]
        );
        size_t first_iter = iterate_cell(iterations, start_complex);
        for (size_t i = 0; i <= (end - start).norm(); i++) {
            vect pos = start + diff * i;
            if (grid_iter((const size_t) pos[0], (const size_t) pos[1]) == NOT_DEFINED) {
                std::complex<double> complex_pos;
                complex_pos.real((pos[0] / grid_iter.x()) * (bounds[1] - bounds[0]) + bounds[0]);
                complex_pos.imag((pos[1] / grid_iter.y()) * (bounds[3] - bounds[2]) + bounds[2]);
                size_t iter = iterate_cell(iterations, complex_pos);
                grid_iter((const size_t) pos[0], (const size_t) pos[1]) = iter;
            }
            all_equal &= (first_iter == grid_iter((const size_t) pos[0], (const size_t) pos[1]));
        }
        return all_equal;
    }

    void process_rectangle(const rectangle &rect, matrix<size_t> &grid_iter, const size_t iterations,
                           const std::array<double, 4> bounds) {
        bool edges_equal = true;
        for (auto &line : rect.sides) {
            // pre-calculate to avoid lazy evaluation skipping
            bool res = process_line(grid_iter, iterations, line, bounds);
            edges_equal = edges_equal && res;
        }
        size_t shortest_edge = std::min(rect.xmax - rect.xmin, rect.ymax - rect.ymin);
        size_t longest_bound = std::max(grid_iter.x(), grid_iter.y());
        // todo improve magic number?
        if (edges_equal && shortest_edge < longest_bound / 2) {
            size_t iter_fill = grid_iter(rect.xmin, rect.ymin);
            for (size_t i = rect.xmin + 1; i < rect.xmax; i++) {
                for (size_t j = rect.ymin + 1; j < rect.ymax; j++) {
                    grid_iter(i, j) = iter_fill;
                }
                // uncomment the following to include visualizations of the recursion in the output
//                grid_iter(i, rect.ymin) = 100;
            }
//            for (size_t j = rect.ymin; j < rect.ymax; j++) {
//                grid_iter(rect.xmin, j) = 100;
//            }
        } else if (rect.xmax - rect.xmin == 2 || rect.ymax - rect.ymin == 2) {
            // handle single line case, because we can't easily split that up into sub-rectangles
            process_rectangle(rectangle(rect.xmin + 1, rect.xmax - 1, rect.ymin + 1, rect.ymax - 1), grid_iter, iterations, bounds);
        } else if (shortest_edge > 1) {
            // must be careful how we round up and down because rectangles are inclusive on all bounds
            process_rectangle(rectangle(rect.xmin + 1, (rect.xmin + rect.xmax) / 2,
                                        rect.ymin + 1, (rect.ymin + rect.ymax) / 2), grid_iter, iterations, bounds);
            process_rectangle(rectangle((rect.xmin + rect.xmax) / 2,
                                        rect.xmax, rect.ymin, (rect.ymin + rect.ymax + 1) / 2), grid_iter, iterations, bounds);
            process_rectangle(rectangle(rect.xmin, (rect.xmin + rect.xmax) / 2,
                                        (rect.ymin + rect.ymax + 1) / 2, rect.ymax), grid_iter, iterations, bounds);
            process_rectangle(rectangle((rect.xmin + rect.xmax + 1) / 2, rect.xmax - 1,
                                        (rect.ymin + rect.ymax + 1) / 2, rect.ymax - 1), grid_iter, iterations, bounds);
        }
    }

    void fast_mandelbrot(size_t iterations, matrix<double> &grid, std::array<double, 4> bounds) {
        matrix<size_t> grid_iter(grid.x(), grid.y(), NOT_DEFINED);
        rectangle starter(0, grid.x() - 1, 0, grid.y() - 1);
        process_rectangle(starter, grid_iter, iterations, bounds);
        for (size_t i = 0; i < grid.x(); ++i) {
            for (size_t j = 0; j < grid.y(); ++j) {
                grid(i, j) = grid_iter(i, j);
            }
        }
    }

}
