// (c) Copyright 2016 Josh Wright
#include <util/debug.h>
#include "fractal_singlethread.h"

namespace image_utils {

    fractal_singlethread::fractal_singlethread(const size_t w, const size_t h) : fractal_base(w, h) {}

    void fractal_singlethread::run() {
        std::fill(iterations.begin(), iterations.end(), NOT_DEFINED);
        pixel_width_x = 2 / iterations.x();
        pixel_width_y = 2 / iterations.y();
        stack.push_back(rectangle(0, iterations.x() / 2, 0, iterations.y() / 2));
        stack.push_back(rectangle(iterations.x() / 2, iterations.x() - 1, 0, iterations.y() / 2));
        stack.push_back(rectangle(0, iterations.x() / 2, iterations.y() / 2, iterations.y() - 1));
        stack.push_back(rectangle(iterations.x() / 2, iterations.x() - 1, iterations.y() / 2, iterations.y() - 1));

        while (!stack.empty()) {
            rectangle current = stack.pop();
            split_rectangle sp = process_rectangle(current);
            if (sp.did_split) {
                stack.push(sp.rectangles[0]);
                stack.push(sp.rectangles[1]);
                stack.push(sp.rectangles[2]);
                stack.push(sp.rectangles[3]);
            }
        }
        if (do_sine_transform) {
            log_transform(iterations);
            sine_transform(iterations, mul);
        }
    }

    fractal_singlethread::fractal_singlethread() {}

    fractal_singlethread::fractal_singlethread(const fractal_singlethread &rhs)
    // stack should always be empty when not running, so actually copying it is probably overkill
            : fractal_base(rhs), stack(rhs.stack) {}

}
