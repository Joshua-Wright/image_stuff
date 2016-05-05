// (c) Copyright 2015 Josh Wright

#include "filters.h"


namespace image_utils {

    void gaussian_helper(const matrix<containers::vect<double, 3>> &in,
                         matrix<containers::vect<double, 3>> &out,
                         const size_t &x, const size_t &y) {
        const size_t wid = 5;
        containers::vect<double, 3> avg{0, 0, 0};
        size_t count = 0;
        for (int dx = -1 * wid; dx < wid; ++dx) {
            for (int dy = -1 * wid; dy < wid; ++dy) {
                /*if inside input matrix*/
//                if ((x + dx) < in.x() && (x + dx) >= 0 &&
//                    (y + dy) < in.y() && (y + dy) >= 0) {
//                    /*add into the weighted average*/
//                    if (dx != 0 && dy != 0) {
//                        avg += in(x + dx, y + dy) / (x * x + y * y);
//                    } else {
//                        avg += in(x + dx, y + dy) / 0.5;
//                    }
//                    ++count;
//                }
                try {
                    avg += in(x + dx, y + dy) / (dx * dx + dy * dy + 1);
//                    if (dx != 0 && dy != 0) {
//                        avg += in(x + dx, y + dy) / (dx * dx + dy * dy);
//                    } else {
//                        avg += in(x + dx, y + dy) / 0.5;
//                    }
                    ++count;
                } catch (...) { }
            }
        }
//        out(x, y) = avg / count;
        out(x, y) = avg;
    }

    matrix<containers::vect<double, 3>> gaussian_blur(
            const matrix<containers::vect<double, 3>> &in) {

        matrix<containers::vect<double, 3>> out(in.x(), in.y());

#pragma omp parallel for schedule(static) collapse(2)
        for (size_t x = 0; x < in.x(); ++x) {
            for (size_t y = 0; y < in.y(); ++y) {
                gaussian_helper(in, out, x, y);
            }
        }

        return out;
    }

}
