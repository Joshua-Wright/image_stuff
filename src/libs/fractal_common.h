// (c) Copyright 2016 Josh Wright
#pragma once

#include "types.h"
#include <map>
#include <functional>
#include <util/debug.h>


namespace image_utils {
    const double NOT_DEFINED = -1.0;

    typedef std::function<complex(complex, complex)> mandelbrot_polynomial_t;

    auto func_standard = [](const complex &z, const complex &c) { return pow(z, 2) + c; };
    auto func_cubic = [](const complex &z, const complex &c) { return pow(z, 3) + c; };
//    auto func_inv_c = [](const complex &z, const complex &c) { return pow(z, 2) + 1.0 / c; };
    auto func_inv_c = [](const complex &z, const complex &c) { return pow(z, 2) + 1.0 / (c - 1.0); };
    auto func_inv_c_parabola = [](const complex &z, const complex &c) { return pow(z, 2) + 1.0 / c + 0.25; };
    auto func_quadratic_rational = [](const complex &z, const complex &c) { return pow(z, 2) + pow(c, 2) / (pow(c, 4) - 0.25); };
    // TODO these don't work
    auto func_lambda = [](const complex &z, const complex &c) {
//        const auto lambda = -sqrt(-4.0 * c - 1.0) - 1.0;
        const auto lambda = pow(c, 2) / 4.0 - c / 2.0;
//        return pow(z, 2) + lambda;
//        return pow(z, 2) * lambda;
        return z * (1.0 - z) - lambda;
//        return lambda * z * (1.0 - z);
    };
    auto func_inv_lambda = [](const complex &z, const complex &c) { return 1.0 / (c * (c - 1.0)); };

    enum polynomial_t {
        STANDARD,
        CUBIC,
        QUADRATIC_RATIONAL,
        INV_C,
        INV_C_PARABOLA,
        LAMBDA,
        INV_LAMBDA,
    };
    const std::map<std::string, polynomial_t> names{
            {"standard",           STANDARD},
            {"cubic",              CUBIC},
            {"inv-c",              INV_C},
            {"quadratic-rational", QUADRATIC_RATIONAL},
            {"inv-c-parabola",     INV_C_PARABOLA},
            {"lambda",             LAMBDA},
            {"inv-lambda",         INV_LAMBDA},
    };

    double fractal_cell(complex z, const complex &c, const size_t m, const bool s, const polynomial_t poly);

    template<typename Func>
    double fractal_cell_(complex z, const complex &c, const size_t max_iterations, const bool smooth, const Func func = func_standard) {
        const double cap = max_iterations * max_iterations;
        for (size_t i = 0; i < max_iterations; i++) {
            z = func(z, c);
            if (norm(z) > cap) {
                if (smooth) {
                    return i - log2(log2(norm(z) + 1) + 1) + 4.0;
                } else {
                    return i;
                }
            }
        }
        return 0.0;
    }

    void sine_transform(matrix<double> &in, const double multiplier = 1, const double rel_phase = 0, bool preserve_zero = true);
    void log_transform(matrix<double> &in, const double multiplier = 1);

    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////

    struct line {
        vec_ull start_point;
        vec_ull end_point;
    };

    struct rectangle {
        size_t xmin, xmax, ymin, ymax;

        rectangle() {};

        rectangle(const size_t x_min, const size_t x_max,
                  const size_t y_min, const size_t y_max) : xmin(x_min), xmax(x_max),
                                                            ymin(y_min), ymax(y_max) {}


        std::array<line, 4> get_sides() const {
            return std::array<line, 4>{
                    line{
                            {xmin, ymin},
                            {xmax, ymin}
                    },
                    line{
                            {xmin, ymin},
                            {xmin, ymax}
                    },
                    line{
                            {xmax, ymin},
                            {xmax, ymax}
                    },
                    line{
                            {xmin, ymax},
                            {xmax, ymax}
                    }
            };
        };
    };

    class rectangle_stack : public std::vector<rectangle> {
    public:

        rectangle_stack(size_t sz) : std::vector<rectangle>() {
            reserve(sz);
        };

        rectangle pop() {
            rectangle ret = back();
            pop_back();
            return ret;
        }

        void push(const rectangle &rect) {
            push_back(rect);
        }
    };


}