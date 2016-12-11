// (c) Copyright 2016 Josh Wright
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <iomanip>
#include "arg_parser.h"
#include "colormaps.h"
#include "generators.h"
#include "io.h"
#include "fractal/fractal_multithread.h"
#include "types.h"
#include <complex>
#include <cmath>
#include <stack>
#include <thread>
#include <vector>
#include "fractal/fractal_multithread.h"
#include "util/debug.h"
#include "util/vect.h"
#include "types.h"

namespace image_utils {

    auto func_standard = [](const complex &z, const complex c) { return pow(z, 2) + c; };

    template<typename T>
    double fractal_cell(complex z, const complex &c, const size_t max_iterations, const bool smooth, const T func = func_standard) {
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

}


int main(int argc, char const *argv[]) {
    using image_utils::fractal_cell;
    using image_utils::complex;
    using image_utils::func_standard;

    const complex z(0, 0);
    const size_t max_iter = 2048;


    const int itr = atoi(argv[1]);
    const double d = 2.0 / itr;

    double total = 0;

    for (double r = -2; r < 2; r += d) {
        for (double i = -2; i < 2; i += d) {
            double cell = fractal_cell(z, complex(r, i), max_iter, true, func_standard);
            total += cell;
        }
    }

    return total == 0;
}
