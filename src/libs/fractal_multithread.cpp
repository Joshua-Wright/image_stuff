//
// Created by j0sh on 5/27/16.
//
#include <complex>
#include <cmath>
#include <stack>
#include <thread>
#include <vector>
#include "fractal_multithread.h"
#include "util/debug.h"
#include "types.h"

namespace image_utils {

    complex complex_circle(const complex center, const double r, const double t) {
        complex pos(std::cos(2 * PI * t), std::sin(2 * PI * t));
        return center + r * pos;
    }

    double fractal_multithread::iterate_cell(const complex pos) {
        if (subsample) {
            double out[] = {0, 0, 0, 0};
            if (is_julia) {
                out[0] = fractal_cell(pos + complex(-pixel_width_x, 0), c, max_iterations, smooth, polynomial);
                out[1] = fractal_cell(pos + complex(pixel_width_x, 0), c, max_iterations, smooth, polynomial);
                out[2] = fractal_cell(pos + complex(0, -pixel_width_y), c, max_iterations, smooth, polynomial);
                out[3] = fractal_cell(pos + complex(0, pixel_width_y), c, max_iterations, smooth, polynomial);
            } else {
                out[0] = fractal_cell(complex(0, 0), pos + complex(-pixel_width_x, 0), max_iterations, smooth, polynomial);
                out[1] = fractal_cell(complex(0, 0), pos + complex(pixel_width_x, 0), max_iterations, smooth, polynomial);
                out[2] = fractal_cell(complex(0, 0), pos + complex(0, -pixel_width_y), max_iterations, smooth, polynomial);
                out[3] = fractal_cell(complex(0, 0), pos + complex(0, pixel_width_y), max_iterations, smooth, polynomial);
            }
            return ((out[0] + out[1]) + (out[2] + out[3])) / 4;
        } else {
            if (is_julia) {
                return fractal_cell(pos, c, max_iterations, smooth, polynomial);
            } else {
                return fractal_cell(complex(0, 0), pos, max_iterations, smooth, polynomial);
            }
        }
    }

    bool fractal_multithread::process_line(const line &l) {
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
                complex complex_pos(
                        (pos[0] * 1.0 / iterations.x()) * (bounds[1] - bounds[0]) + bounds[0],
                        bounds[3] - (pos[1] * 1.0 / iterations.y()) * (bounds[3] - bounds[2])
                );
                iterations(pos) = iterate_cell(complex_pos);
            }
            if (iterations(pos) != iterations(start)) {
                out = false;
            }
        }
        return out;
    }

    fractal_multithread::split_rectangle fractal_multithread::process_rectangle(rectangle r) {
        bool edges_equal = true;
        for (auto &side : r.get_sides()) {
            // pre-calculate to avoid lazy evaluation skipping
            bool res = process_line(side);
            edges_equal = edges_equal && res;
        }
        size_t shortest_edge = std::min(r.xmax - r.xmin, r.ymax - r.ymin);
        size_t longest_bound = std::max(iterations.x(), iterations.y());
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

    matrix<double> fractal_multithread::run() {
        const size_t stack_size = iterations.x() * iterations.y();
        matrix<double> grid(iterations.x(), iterations.y());

        std::vector<rectangle> rectange_stack;
        // four quadrants for starting
        rectange_stack.push_back(rectangle(0, iterations.x() / 2, 0, iterations.y() / 2));
        rectange_stack.push_back(rectangle(iterations.x() / 2, iterations.x() - 1, 0, iterations.y() / 2));
        rectange_stack.push_back(rectangle(0, iterations.x() / 2, iterations.y() / 2, iterations.y() - 1));
        rectange_stack.push_back(rectangle(iterations.x() / 2, iterations.x() - 1, iterations.y() / 2, iterations.y() - 1));

        rectange_stack.reserve(stack_size);
        while (!rectange_stack.empty()) {
            std::vector<rectangle> new_stack;
            new_stack.reserve(stack_size);
#pragma omp parallel for schedule(dynamic)
            for (size_t i = 0; i < rectange_stack.size(); i++) {
                auto v = process_rectangle(rectange_stack[i]);

                if (v.did_split) {
#pragma omp critical
                    {
                        new_stack.push_back(v.rectangles[0]);
                        new_stack.push_back(v.rectangles[1]);
                        new_stack.push_back(v.rectangles[2]);
                        new_stack.push_back(v.rectangles[3]);
                    }
                }
            }
            rectange_stack = new_stack;
        }

        if (do_sine_transform) {
            grid = iterations;
            log_transform(iterations);
            sine_transform(grid, mul);
        } else {
#pragma omp parallel for schedule(static) collapse(2)
            for (size_t i = 0; i < grid.x(); ++i) {
                for (size_t j = 0; j < grid.y(); ++j) {
                    grid(i, j) = iterations(i, j);
                }
            }

        }
        if (do_grid) {
            /*get max iteration (that was used) and use that*/
            double grid_color = *std::max_element(iterations.begin(), iterations.end());
#pragma omp parallel for schedule(static) collapse(2)
            for (size_t i = 0; i < grid.x(); ++i) {
                for (size_t j = 0; j < grid.y(); ++j) {
                    if (grid_mask(i, j)) {
                        grid(i, j) = grid_color;
                    }
                }
            }
        }
        return grid;
    }

    fractal_multithread::fractal_multithread(const size_t w, const size_t h) : iterations(w, h, NOT_DEFINED), grid_mask(0, 0),
                                                                               pixel_width_x(2 / w), pixel_width_y(2 / h) {}


    void fractal_multithread::set_do_grid(bool do_grid) {
        fractal_multithread::do_grid = do_grid;
        if (do_grid) {
            grid_mask = matrix<bool>(iterations.x(), iterations.y(), false);
        }
    }

    void fractal_multithread::set_is_julia(bool is_julia) {
        fractal_multithread::is_julia = is_julia;
    }

    void fractal_multithread::set_c(const complex &c) {
        fractal_multithread::c = c;
    }

    void fractal_multithread::set_max_iterations(size_t max_iterations) {
        fractal_multithread::max_iterations = max_iterations;
    }

    void fractal_multithread::set_smooth(bool smooth) {
        fractal_multithread::smooth = smooth;
    }

    void fractal_multithread::set_do_sine_transform(bool do_sine_transform) {
        this->do_sine_transform = do_sine_transform;
    }

    void fractal_multithread::set_zoom(vec2 center, double zoom) {
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

    void fractal_multithread::set_subsample(bool subsample) {
        fractal_multithread::subsample = subsample;
    }

    void fractal_multithread::set_polynomial(polynomial_t polynomial) {
        polynomial = polynomial;
    }

    void fractal_multithread::set_polynomial(const std::string &name) {
        auto iter_name = names.find(name);
        if (iter_name == names.end()) {
            polynomial = STANDARD;
        } else {
            polynomial = iter_name->second;
        }
    }

    void fractal_multithread::set_mul(double mul) {
        fractal_multithread::mul = mul;
    }

}
