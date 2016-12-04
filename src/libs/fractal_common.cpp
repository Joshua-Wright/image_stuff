// (c) Copyright 2016 Josh Wright
#include "fractal_common.h"

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

};

