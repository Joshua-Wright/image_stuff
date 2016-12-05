// (c) Copyright 2016 Josh Wright
#include "fractal_common.h"
#include "generators.h"

namespace image_utils {

    double fractal_cell(complex z, const complex &c, const size_t m, const bool s, const polynomial_t poly) {
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


};

