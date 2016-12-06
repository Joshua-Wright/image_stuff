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

    fractal_multithread::fractal_multithread(const size_t w, const size_t h) : fractal_base(w, h) {}

    fractal_multithread::fractal_multithread() {};

}
