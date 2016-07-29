//
// Created by j0sh on 5/27/16.
//
#include <complex>
#include <cmath>
#include <stack>
#include <thread>
#include <vector>
#include "fractal.h"
#include "util/debug.h"
#include "util/vect.h"
#include "types.h"

namespace image_utils {
    const double NOT_DEFINED = -1.0;

    auto func_standard_plane = [](const complex &z, const complex c) { return pow(z, 2) + c; };

    template<typename T>
    double fractal_cell(complex z, const complex &c, const size_t max_iterations, const bool smooth, const T func = func_standard_plane) {
        for (size_t i = 0; i < max_iterations; i++) {
            z = func(z, c);
            if (norm(z) > max_iterations * max_iterations) {
                if (smooth) {
                    return i - log2(log2(norm(z))) + 4.0;
                } else {
                    return i;
                }
            }
        }
        return 0.0;
    }


    complex complex_circle(const complex center, const double r, const double t) {
        complex pos(std::cos(2 * PI * t), std::sin(2 * PI * t));
        return center + r * pos;
    }

    void check_bounds(const std::array<double, 4> &bounds) {
        using std::runtime_error;
        if (bounds[1] <= bounds[0]) {
            throw runtime_error("xmin must be less than xmax");
        } else if (bounds[3] <= bounds[2]) {
            throw runtime_error("ymin must be less than ymax");
        }
    }

    double fractal::iterate_cell(const complex pos) {
        if (subsample) {
            double out[] = {0, 0, 0, 0};
            if (is_julia) {
                out[0] = fractal_cell(pos + complex(-pixel_width_x, 0), c, max_iterations, smooth, func_standard_plane);
                out[1] = fractal_cell(pos + complex(pixel_width_x, 0), c, max_iterations, smooth, func_standard_plane);
                out[2] = fractal_cell(pos + complex(0, -pixel_width_y), c, max_iterations, smooth, func_standard_plane);
                out[3] = fractal_cell(pos + complex(0, pixel_width_y), c, max_iterations, smooth, func_standard_plane);
            } else {
                out[0] = fractal_cell(complex(0, 0), pos + complex(-pixel_width_x, 0), max_iterations, smooth, func_standard_plane);
                out[1] = fractal_cell(complex(0, 0), pos + complex(pixel_width_x, 0), max_iterations, smooth, func_standard_plane);
                out[2] = fractal_cell(complex(0, 0), pos + complex(0, -pixel_width_y), max_iterations, smooth, func_standard_plane);
                out[3] = fractal_cell(complex(0, 0), pos + complex(0, pixel_width_y), max_iterations, smooth, func_standard_plane);
            }
            return ((out[0] + out[1]) + (out[2] + out[3])) / 4;
        } else {
            if (is_julia) {
                return fractal_cell(pos, c, max_iterations, smooth, func_standard_plane);
            } else {
                return fractal_cell(complex(0, 0), pos, max_iterations, smooth, func_standard_plane);
            }
        }
    }

    bool fractal::process_line(const fractal::line &l) {
        const vec_ull start = l.start_point;
        const vec_ull end = l.end_point;
        // handle lines containing only a single pixel
        const vec_ull diff = ((start - end) != vec_ull{0, 0}) ? (end - start).unitV() : vec_ull{0, 0};
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
        }

        for (size_t i = 1; i <= length; i++) {
            vec_ull pos = start + diff * i;
            if (iterations(pos) != iterations(start)) {
                return false;
            }
        }
        return true;
    }

    fractal::split_rectangle fractal::process_rectangle(fractal::rectangle r) {
        bool edges_equal = true;
        for (auto &line : r.sides) {
            // pre-calculate to avoid lazy evaluation skipping
            bool res = process_line(line);
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
            if (do_grid) {
                for (size_t j = r.ymin; j < r.ymax; j++) {
                    grid_mask(r.xmin, j) = true;
                }
                for (size_t i = r.xmin; i < r.xmax; i++) {
                    grid_mask(i, r.ymin) = true;
                }
            }
        } else if (!edges_equal && shortest_edge > 1) {
            // must be careful how we round up and down because rectangles are inclusive on all bounds
            return {true,
                    {
                            rectangle(r.xmin, (r.xmin + r.xmax) / 2, r.ymin, (r.ymin + r.ymax) / 2),
                            rectangle((r.xmin + r.xmax) / 2, r.xmax, r.ymin, (r.ymin + r.ymax) / 2),
                            rectangle(r.xmin, (r.xmin + r.xmax) / 2, (r.ymin + r.ymax) / 2, r.ymax),
                            rectangle((r.xmin + r.xmax) / 2, r.xmax, (r.ymin + r.ymax) / 2, r.ymax),
                    },
            };
        }
        return {false, {}};
    }

    matrix<double> fractal::run() {
        matrix<double> grid(iterations.x(), iterations.y());
        rectangle starter(0, iterations.x() - 1, 0, iterations.y() - 1);

        std::vector<rectangle> rectange_stack(1, starter);
        while (!rectange_stack.empty()) {
            std::vector<rectangle> new_stack;
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
#pragma omp parallel for schedule(static) collapse(2)
            for (size_t i = 0; i < grid.x(); ++i) {
                for (size_t j = 0; j < grid.y(); ++j) {
                    grid(i, j) = pow(sin(log2(iterations(i, j) + 1) * PI / 4), 2);
                }
            }
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

    fractal::fractal(const size_t w, const size_t h) : iterations(w, h, NOT_DEFINED), grid_mask(0, 0),
                                                       pixel_width_x(2 / w), pixel_width_y(2 / h) {}


    void fractal::set_do_grid(bool do_grid) {
        fractal::do_grid = do_grid;
        if (do_grid) {
            grid_mask = matrix<bool>(iterations.x(), iterations.y(), false);
        }
    }

    void fractal::set_is_julia(bool is_julia) {
        fractal::is_julia = is_julia;
    }

    void fractal::set_c(const complex &c) {
        fractal::c = c;
    }

    void fractal::set_max_iterations(size_t max_iterations) {
        fractal::max_iterations = max_iterations;
    }

    void fractal::set_smooth(bool smooth) {
        fractal::smooth = smooth;
    }

    void fractal::set_do_sine_transform(bool do_sine_transform) {
        this->do_sine_transform = do_sine_transform;
    }

    void fractal::set_zoom(vec2 center, double zoom) {
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

    void fractal::set_subsample(bool subsample) {
        fractal::subsample = subsample;
    }

    fractal::rectangle::rectangle(const size_t x_min, const size_t x_max, const size_t y_min, const size_t y_max) : xmin(x_min),
                                                                                                                    xmax(x_max),
                                                                                                                    ymin(y_min),
                                                                                                                    ymax(y_max) {
        corners[0] = {x_min, y_min};
        corners[1] = {x_max, y_min};
        corners[2] = {x_min, y_max};
        corners[3] = {x_max, y_max};

        sides[0] = {corners[0], corners[1]};
        sides[1] = {corners[0], corners[2]};
        sides[2] = {corners[1], corners[3]};
        sides[3] = {corners[2], corners[3]};
    }

    fractal::rectangle::rectangle() {}
}