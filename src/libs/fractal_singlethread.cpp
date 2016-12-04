// (c) Copyright 2016 Josh Wright
#include <util/debug.h>
#include "fractal_singlethread.h"
#include "fractal_common.h"
#include "io.h"

namespace image_utils {


    fractal_singlethread::fractal_singlethread(const size_t w, const size_t h) : iterations(w, h, NOT_DEFINED),
                                                                                 pixel_width_x(2 / w), pixel_width_y(2 / h) {}

    matrix<double> fractal_singlethread::run() {
        std::fill(iterations.begin(), iterations.end(), NOT_DEFINED);
        pixel_width_x = 2 / iterations.x();
        pixel_width_y = 2 / iterations.y();
        matrix<double> grid(iterations.x(), iterations.y(), 0);
        stack.push_back(rectangle(0, iterations.x() / 2, 0, iterations.y() / 2));
        stack.push_back(rectangle(iterations.x() / 2, iterations.x() - 1, 0, iterations.y() / 2));
        stack.push_back(rectangle(0, iterations.x() / 2, iterations.y() / 2, iterations.y() - 1));
        stack.push_back(rectangle(iterations.x() / 2, iterations.x() - 1, iterations.y() / 2, iterations.y() - 1));

        while (!stack.empty()) {
            rectangle current = stack.pop();
            process_rectangle(current);
        }
        if (do_sine_transform) {
            for (size_t i = 0; i < grid.x(); ++i) {
                for (size_t j = 0; j < grid.y(); ++j) {
                    grid(i, j) = pow(sin(log2(iterations(i, j) + 1) * PI / 4 * mul), 2);
                }
            }
        } else {
            for (size_t i = 0; i < grid.x(); ++i) {
                for (size_t j = 0; j < grid.y(); ++j) {
                    grid(i, j) = iterations(i, j);
                }
            }
        }
        return grid;
    }

    double fractal_singlethread::iterate_cell(const complex pos) {
        if (subsample) {
            double out[] = {0, 0, 0, 0};
            if (is_julia) {
                out[0] = fractal_cell_(pos + complex(-pixel_width_x, 0), c, max_iterations, smooth, polynomial);
                out[1] = fractal_cell_(pos + complex(pixel_width_x, 0), c, max_iterations, smooth, polynomial);
                out[2] = fractal_cell_(pos + complex(0, -pixel_width_y), c, max_iterations, smooth, polynomial);
                out[3] = fractal_cell_(pos + complex(0, pixel_width_y), c, max_iterations, smooth, polynomial);
            } else {
                out[0] = fractal_cell_(complex(0, 0), pos + complex(-pixel_width_x, 0), max_iterations, smooth, polynomial);
                out[1] = fractal_cell_(complex(0, 0), pos + complex(pixel_width_x, 0), max_iterations, smooth, polynomial);
                out[2] = fractal_cell_(complex(0, 0), pos + complex(0, -pixel_width_y), max_iterations, smooth, polynomial);
                out[3] = fractal_cell_(complex(0, 0), pos + complex(0, pixel_width_y), max_iterations, smooth, polynomial);
            }
            return ((out[0] + out[1]) + (out[2] + out[3])) / 4;
        } else {
            if (is_julia) {
                return fractal_cell_(pos, c, max_iterations, smooth, polynomial);
            } else {
                return fractal_cell_(complex(0, 0), pos, max_iterations, smooth, polynomial);
            }
        }
    }

    bool fractal_singlethread::process_line(const line &l) {
        const vec_ull start = l.start_point;
        const vec_ull end = l.end_point;
        // handle lines containing only a single pixel
        const vec_ull diff = (start != end) ? (end - start).unitV() : vec_ull{0, 0};
        const size_t length = (end - start).norm();

        for (size_t i = 0; i <= length; i++) {
            vec_ull pos = start + diff * i;
            if (iterations(pos) == NOT_DEFINED) {
                // imaginary axis is different because it points opposite our +y axis
                complex complex_pos(
                        (pos[0] * 1.0 / iterations.x()) * (bounds[1] - bounds[0]) + bounds[0],
                        bounds[3] - (pos[1] * 1.0 / iterations.y()) * (bounds[3] - bounds[2])
                );
                iterations(pos) = iterate_cell(complex_pos);
            }
            if (iterations(pos) != iterations(start)) {
                return false;
            }
        }
        return true;
    }

    void fractal_singlethread::process_rectangle(const rectangle &r) {
        bool edges_equal = true;
        for (auto &side : r.get_sides()) {
            // pre-calculate to avoid lazy evaluation skipping
            bool res = process_line(side);
            edges_equal = edges_equal && res;
        }
        size_t shortest_edge = std::min(r.xmax - r.xmin, r.ymax - r.ymin);
        size_t longest_bound = std::max(iterations.x(), iterations.y());
        // todo improve magic number?
        if (edges_equal && shortest_edge < longest_bound / 2) {
            double iter_fill = iterations(r.xmin, r.ymin);
            for (size_t i = r.xmin; i <= r.xmax; i++) {
                for (size_t j = r.ymin; j <= r.ymax; j++) {
                    iterations(i, j) = iter_fill;
                }
            }
        } else if (!edges_equal && shortest_edge > 1) {
            // must be careful how we round up and down because rectangles are inclusive on all bounds
            stack.push(rectangle(r.xmin, (r.xmin + r.xmax) / 2, r.ymin, (r.ymin + r.ymax) / 2));
            stack.push(rectangle((r.xmin + r.xmax) / 2, r.xmax, r.ymin, (r.ymin + r.ymax) / 2));
            stack.push(rectangle(r.xmin, (r.xmin + r.xmax) / 2, (r.ymin + r.ymax) / 2, r.ymax));
            stack.push(rectangle((r.xmin + r.xmax) / 2, r.xmax, (r.ymin + r.ymax) / 2, r.ymax));
        }
    }

    void fractal_singlethread::set_zoom(vec2 center, double zoom) {
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
}

