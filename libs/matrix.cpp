//
// Created by j0sh on 11/13/15.
//
#include "matrix.h"

namespace image_utils {

    template<>
    void matrix<long double>::initialize_zero() {
        for (long double &d : *this) {
            d = 0.0;
        }
    }


    template<>
    void matrix<double>::initialize_zero() {
        for (double &d : *this) {
            d = 0.0;
        }
    }

    template<>
    void matrix<float>::initialize_zero() {
        for (float &d : *this) {
            d = 0.0;
        }
    }

}